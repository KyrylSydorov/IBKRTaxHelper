// Kyryl Sydorov, 2025

#include "CommonIncludes.h"

#include "CsvParser.h"
#include "RateStorage.h"
#include "StockTrade.h"

static const string ActivityFileName = "Activity.csv";
static const string RatesFileName = "Rates.csv";

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
    FRateStorage RateStorage(RatesFileName);
    
    FFileLines Lines = FCsvParser::ParseFile("Activity.csv");

    FFileLine TradeLine = { "Trades", "Data", "Order", "Stocks" };

    vector<FStockTrade> Trades;

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
            const dec4 TotalBoughtSumUAH = TotalBoughtSum * RateStorage.GetRate(Trade.Date, Trade.Currency);
            Trade.PriceUAH = TotalBoughtSumUAH / Trade.Quantity;
            Trades.push_back(Trade);
            continue;
        }

        const dec4 TotalSoldSum = Trade.Price * -Trade.Quantity - Trade.Fee;
        const dec4 TotalSoldSumUAH = TotalSoldSum * RateStorage.GetRate(Trade.Date, Trade.Currency);
        Trade.PriceUAH = TotalSoldSumUAH / -Trade.Quantity;

        while (Trade.QuantityLeft < 0)
        {
            vector<FStockTrade>::iterator TradeIt = FindTrade(Trades, Trade.Symbol);
            if (TradeIt == Trades.end())
            {
                __debugbreak();
                cout << "Trade: " << Trade.Symbol << ' '
                    << "Sold: " << -Trade.QuantityLeft << ' '
                    << "\tPrice: " << Trade.PriceUAH << ' '
                    << "\tBought: 0 "
                    << "\tPrice: 0 "
                    << "\tEarned: 0\n";
                break;
            }

            FStockTrade& UsedTrade = *TradeIt;
            
            if (UsedTrade.QuantityLeft <= -Trade.QuantityLeft)
            {
                const dec4 SumSold = UsedTrade.QuantityLeft * Trade.PriceUAH;
                const dec4 SumBought = UsedTrade.QuantityLeft * UsedTrade.PriceUAH;
                cout << "Trade: " << UsedTrade.Symbol << ' '
                << "Sold: " << UsedTrade.QuantityLeft << ' '
                << "\tPrice: " << Trade.PriceUAH << ' '
                << "\tSumSold: " << SumSold << ' '
                << "\tBought: " << UsedTrade.QuantityLeft << ' '
                << "\tPrice: " << UsedTrade.PriceUAH << ' '
                << "\tSumBought: " << SumBought << ' '
                << "\tEarned: " << SumSold - SumBought << '\n';

                Trade.QuantityLeft += UsedTrade.QuantityLeft;
                Trades.erase(TradeIt);
                continue;
            }

            const dec4 SumSold = -Trade.QuantityLeft * Trade.PriceUAH;
            const dec4 SumBought = -Trade.QuantityLeft * UsedTrade.PriceUAH;
            cout << "Trade: " << UsedTrade.Symbol << ' '
                << "Sold: " << -Trade.QuantityLeft << ' '
                << "\tPrice: " << Trade.PriceUAH << ' '
                << "\tSumSold: " << SumSold << ' '
                << "\tBought: " << -Trade.QuantityLeft << ' '
                << "\tPrice: " << UsedTrade.PriceUAH << ' '
                << "\tSumBought: " << SumBought << ' '
                << "\tEarned: " << SumSold - SumBought << '\n';
            UsedTrade.QuantityLeft += Trade.QuantityLeft;
            Trade.QuantityLeft = 0.0;
        }
    }
}