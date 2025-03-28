// Kyryl Sydorov, 2025

#include "IBKRManager.h"

#include "CsvParser.h"
#include "RateProvider.h"

namespace IBKRManager
{
    static vector<FStockTrade>::iterator FindTrade(vector<FStockTrade>& Trades, const string& Symbol)
    {
        for (auto It = Trades.begin(); It != Trades.end(); ++It)
        {
            if (It->Symbol == Symbol)
            {
                return It;
            }
        }
        return Trades.end();
    }
}

FIBKRManager::FIBKRManager(unique_ptr<IRateProvider> RateProvider)
    : RateProvider(move(RateProvider))
{
}

bool FIBKRManager::ParseStatement(const FFileLines& Lines)
{
    using FProcessorSignature = bool (FIBKRManager::*)(const FFileLine&);
    constexpr FProcessorSignature Processors [] = {
        &FIBKRManager::TryProcessTrade,
        &FIBKRManager::TryProcessDividend,
        &FIBKRManager::TryProcessSYEP,
        &FIBKRManager::TryProcessBondRedemption,
        &FIBKRManager::TryProcessCouponPayment,
    };
    
    for (const FFileLine& Line : Lines)
    {
        for (const FProcessorSignature Processor : Processors)
        {
            if ((this->*Processor)(Line))
            {
                break;
            }
        }
    }

    return true;
}

void FIBKRManager::PrintEverything() const
{
    for (int Year = MinYear; Year <= MaxYear; ++Year)
    {
        cout << "Year: " << Year << "\n";
        PrintMatchedTradesByYear(Year);
        PrintDividendsByYear(Year);
        PrintOtherAccrualsByYear(Year);
        cout << "===============================================\n";
    }
}

void FIBKRManager::PrintMatchedTrades() const
{
    for (const auto& [Year, MatchedTrades] : MatchedTradesByYear)
    {
        cout << "Year: " << Year << "\n";
        PrintMatchedTradesByYear(Year);
        cout << "===============================================\n";
    }
}

void FIBKRManager::PrintDividends() const
{
    for (const auto& [Year, Dividends] : DividendsByYear)
    {
        cout << "Year: " << Year << "\n";
        PrintDividendsByYear(Year);
        cout << "===============================================\n";
    }
}

void FIBKRManager::PrintOtherAccruals() const
{
    for (const auto& [Year, OtherAccruals] : OtherAccrualsByYear)
    {
        cout << "Year: " << Year << "\n";
        PrintOtherAccrualsByYear(Year);
        cout << "===============================================\n";
    }
}

bool FIBKRManager::TryProcessTrade(const FFileLine& Line)
{
    static const FFileLine TradeLine = { "Trades", "Data", "Order", "Stocks" };
    
    if (!FCsvParser::StartsWith(Line, TradeLine))
    {
        return false;
    }    
        
    FStockTrade Trade = ParseTrade(Line);
    Trade.QuantityLeft = Trade.Quantity;

    if (Trade.Quantity > 0)
    {
        const dec4 TotalBoughtSum = Trade.Price * Trade.Quantity + Trade.Fee;
        const dec4 TotalBoughtSumUAH = TotalBoughtSum * RateProvider->GetRate(Trade.Date, Trade.Currency);
        Trade.PriceUAH = TotalBoughtSumUAH / Trade.Quantity;
        Trades.push_back(Trade);
        return true;
    }

    const dec4 TotalSoldSum = Trade.Price * -Trade.Quantity - Trade.Fee;
    const dec4 TotalSoldSumUAH = TotalSoldSum * RateProvider->GetRate(Trade.Date, Trade.Currency);
    Trade.PriceUAH = TotalSoldSumUAH / -Trade.Quantity;

    while (Trade.QuantityLeft < 0)
    {
        vector<FStockTrade>::iterator TradeIt = IBKRManager::FindTrade(Trades, Trade.Symbol);
        if (TradeIt == Trades.end())
        {
            __debugbreak();
            FMatchedTrade MatchedTrade{ .Symbol = Trade.Symbol, .Quantity = -Trade.QuantityLeft, .PriceSold = Trade.PriceUAH };
            cout << "UNMATCHED TRADE: " << MatchedTrade << "\n";
            break;
        }

        FStockTrade& UsedTrade = *TradeIt;
            
        if (UsedTrade.QuantityLeft <= -Trade.QuantityLeft)
        {
            FMatchedTrade MatchedTrade{
                .Symbol = UsedTrade.Symbol,
                .Quantity = UsedTrade.QuantityLeft,
                .DateBought = UsedTrade.Date,
                .PriceBought = UsedTrade.PriceUAH,
                .DateSold = Trade.Date,
                .PriceSold = Trade.PriceUAH,
            };
            MinYear = std::min(MinYear, MatchedTrade.DateSold.Year);
            MaxYear = std::max(MaxYear, MatchedTrade.DateSold.Year);
            MatchedTradesByYear[MatchedTrade.DateSold.Year].emplace_back(move(MatchedTrade));

            Trade.QuantityLeft += UsedTrade.QuantityLeft;
            Trades.erase(TradeIt);
            continue;
        }
        FMatchedTrade MatchedTrade{
            .Symbol = UsedTrade.Symbol,
            .Quantity = -Trade.QuantityLeft,
            .DateBought = UsedTrade.Date,
            .PriceBought = UsedTrade.PriceUAH,
            .DateSold = Trade.Date,
            .PriceSold = Trade.PriceUAH,
        };
        MinYear = std::min(MinYear, MatchedTrade.DateSold.Year);
        MaxYear = std::max(MaxYear, MatchedTrade.DateSold.Year);
        MatchedTradesByYear[MatchedTrade.DateSold.Year].emplace_back(move(MatchedTrade));
            
        UsedTrade.QuantityLeft += Trade.QuantityLeft;
        Trade.QuantityLeft = 0.0;
    }

    return true;
}

bool FIBKRManager::TryProcessDividend(const FFileLine& Line)
{
    static const FFileLine DividendLine = { "Dividends", "Data" };
    static const FFileLine DividendTotalLine = { "Dividends", "Data", "Total" };
    
    if (!FCsvParser::StartsWith(Line, DividendLine) || FCsvParser::StartsWith(Line, DividendTotalLine))
    {
        return false;
    }

    FDividend Dividend = ParseDividend(Line);
    Dividend.AmountUAH = Dividend.Amount * RateProvider->GetRate(Dividend.Date, Dividend.Currency);

    MinYear = std::min(MinYear, Dividend.Date.Year);
    MaxYear = std::max(MaxYear, Dividend.Date.Year);
    DividendsByYear[Dividend.Date.Year].emplace_back(move(Dividend));
    return true;
}

bool FIBKRManager::TryProcessSYEP(const FFileLine& Line)
{
    static const FFileLine SYEPLine = { "Interest", "Data" };
    
    if (!FCsvParser::StartsWith(Line, SYEPLine))
    {
        return false;
    }

    if (Line[4].find("SYEP") == string::npos)
    {
        return false;
    }

    FOtherAccrual Accrual;
    Accrual.Currency = Line[2];
    ParseDateIBKR(Line[3], Accrual.Date);
    Accrual.Description = Line[4];
    Accrual.Amount = std::stod(Line[5]);
    Accrual.AmountUAH = Accrual.Amount * RateProvider->GetRate(Accrual.Date, Accrual.Currency);

    MinYear = std::min(MinYear, Accrual.Date.Year);
    MaxYear = std::max(MaxYear, Accrual.Date.Year);
    OtherAccrualsByYear[Accrual.Date.Year].emplace_back(move(Accrual));

    return true;
}

bool FIBKRManager::TryProcessBondRedemption(const FFileLine& Line)
{
    static const FFileLine BondLine = { "Corporate Actions", "Data", "Bonds" };

    if (!FCsvParser::StartsWith(Line, BondLine))
    {
        return false;
    }

    FOtherAccrual Accrual;
    Accrual.Currency = Line[3];
    ParseDateIBKR(Line[4], Accrual.Date);
    Accrual.Description = Line[6];
    Accrual.Amount = std::stod(Line[10]);
    Accrual.AmountUAH = Accrual.Amount * RateProvider->GetRate(Accrual.Date, Accrual.Currency);

    MinYear = std::min(MinYear, Accrual.Date.Year);
    MaxYear = std::max(MaxYear, Accrual.Date.Year);
    OtherAccrualsByYear[Accrual.Date.Year].emplace_back(move(Accrual));

    return true;
}

bool FIBKRManager::TryProcessCouponPayment(const FFileLine& Line)
{
    static const FFileLine CouponLine = { "Interest", "Data" };
    
    if (!FCsvParser::StartsWith(Line, CouponLine))
    {
        return false;
    }

    if (Line[4].find("Coupon") == string::npos)
    {
        return false;
    }

    FOtherAccrual Accrual;
    Accrual.Currency = Line[2];
    ParseDateIBKR(Line[3], Accrual.Date);
    Accrual.Description = Line[4];
    Accrual.Amount = std::stod(Line[5]);
    Accrual.AmountUAH = Accrual.Amount * RateProvider->GetRate(Accrual.Date, Accrual.Currency);

    MinYear = std::min(MinYear, Accrual.Date.Year);
    MaxYear = std::max(MaxYear, Accrual.Date.Year);
    OtherAccrualsByYear[Accrual.Date.Year].emplace_back(move(Accrual));

    return true;
}


void FIBKRManager::PrintMatchedTradesByYear(int Year) const
{
    const auto It = MatchedTradesByYear.find(Year);
    if (It == MatchedTradesByYear.end())
    {
        cout << "No trades for year " << Year << "\n";
        return;
    }

    const vector<FMatchedTrade>& MatchedTrades = It->second;
    
    dec4 TotalProfit = 0;
        
    for (const FMatchedTrade& MatchedTrade : MatchedTrades)
    {
        cout << MatchedTrade;
        TotalProfit += MatchedTrade.GetProfit();
    }

    cout << "Trade profit: " << TotalProfit << "\n";
}

void FIBKRManager::PrintDividendsByYear(int Year) const
{
    const auto It = DividendsByYear.find(Year);
    if (It == DividendsByYear.end())
    {
        cout << "No dividends for year " << Year << "\n";
        return;
    }

    const vector<FDividend>& Dividends = It->second;
    
    dec4 TotalDividends = 0;
        
    for (const FDividend& Dividend : Dividends)
    {
        cout << Dividend;
        TotalDividends += Dividend.AmountUAH;
    }

    cout << "Total dividends: " << TotalDividends << "\n";
}

void FIBKRManager::PrintOtherAccrualsByYear(int Year) const
{
    const auto It = OtherAccrualsByYear.find(Year);
    if (It == OtherAccrualsByYear.end())
    {
        cout << "No accruals for year " << Year << "\n";
        return;
    }
    
    const vector<FOtherAccrual>& OtherAccruals = It->second;
    
    dec4 TotalAccruals = 0;
        
    for (const FOtherAccrual& Accrual : OtherAccruals)
    {
        cout << Accrual;
        TotalAccruals += Accrual.AmountUAH;
    }

    cout << "Total accruals: " << TotalAccruals << "\n";
}
