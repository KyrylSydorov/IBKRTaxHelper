// Kyryl Sydorov, 2025

#include "StockTrade.h"

FStockTrade ParseTrade(const FFileLine& Line)
{
    FStockTrade Result;
    
    if (Line.size() != 16)
    {
        return Result;
    }

    Result.Currency = Line[4];
    Result.Symbol = Line[5];
    ParseDateIBKR(Line[6], Result.Date);
    Result.Quantity = std::stod(Line[7]);
    Result.Price = std::stod(Line[8]);

    double FeeRaw = -std::stod(Line[11]);
    Result.Fee = rint(FeeRaw * 100.0) / 100.0;

    return Result;
}
