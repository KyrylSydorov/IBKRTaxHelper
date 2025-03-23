// Kyryl Sydorov, 2025

#include "CommonIncludes.h"

#include "CsvParser.h"
#include "MatchedTrade.h"
#include "StockTrade.h"
#include "RateProvider.h"

#if USE_NBU_API
#include "NBURateProvider.h"
#else
#include "RateStorage.h"
#endif

vector<FStockTrade>::iterator FindTrade(vector<FStockTrade>& Trades, const string& Symbol)
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

int main()
{
    unique_ptr<IRateProvider> RateProvider;
#if USE_NBU_API
    RateProvider = make_unique<FNBURateProvider>();
#else
    static const string RatesFileName = "Rates.csv";
    RateProvider = make_unique<FRateStorage>(RatesFileName);
#endif
    
    FFileLines Lines = FCsvParser::ParseFolder("ActivityStatements");

    FFileLine TradeLine = { "Trades", "Data", "Order", "Stocks" };

    vector<FStockTrade> Trades;

    map<int, vector<FMatchedTrade>> MatchedTradesByYear;

    for (const FFileLine& Line : Lines)
    {
        if (!FCsvParser::StartsWith(Line, TradeLine))
        {
            continue;
        }
        
        FStockTrade Trade = ParseTrade(Line);
        Trade.QuantityLeft = Trade.Quantity;

        if (Trade.Quantity > 0)
        {
            const dec4 TotalBoughtSum = Trade.Price * Trade.Quantity + Trade.Fee;
            const dec4 TotalBoughtSumUAH = TotalBoughtSum * RateProvider->GetRate(Trade.Date, Trade.Currency);
            Trade.PriceUAH = TotalBoughtSumUAH / Trade.Quantity;
            Trades.push_back(Trade);
            continue;
        }

        const dec4 TotalSoldSum = Trade.Price * -Trade.Quantity - Trade.Fee;
        const dec4 TotalSoldSumUAH = TotalSoldSum * RateProvider->GetRate(Trade.Date, Trade.Currency);
        Trade.PriceUAH = TotalSoldSumUAH / -Trade.Quantity;

        while (Trade.QuantityLeft < 0)
        {
            vector<FStockTrade>::iterator TradeIt = FindTrade(Trades, Trade.Symbol);
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
            MatchedTradesByYear[MatchedTrade.DateSold.Year].emplace_back(move(MatchedTrade));
            
            UsedTrade.QuantityLeft += Trade.QuantityLeft;
            Trade.QuantityLeft = 0.0;
        }
    }

    for (const auto& [Year, MatchedTrades] : MatchedTradesByYear)
    {
        cout << "Year: " << Year << "\n";

        dec4 TotalProfit = 0;
        
        for (const FMatchedTrade& MatchedTrade : MatchedTrades)
        {
            cout << MatchedTrade;
            TotalProfit += MatchedTrade.GetProfit();
        }

        cout << "Total profit: " << TotalProfit << "\n";
        cout << "===============================================\n";
    }

    
}