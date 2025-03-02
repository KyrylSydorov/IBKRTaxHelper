// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

// Stock trade format:
// [0] - Trades,
// [1] - Data,
// [2] - Order,
// [3] - Stocks,
// [4] - USD,
// [5] - RSP,
// [6] - "2024-07-02, 10:04:33",
// [7] - 0.75,
// [8] - 163.28,
// [9] - 163.68,
// [10] - -122.46,
// [11] - -1,
// [12] - 123.46,
// [13] - 0,
// [14] - 0.3,
// [15] - O;RP

struct FStockTrade
{
    string Currency;
    string Symbol;
    FDate Date;
    dec4 Quantity = 0;
    dec4 Price = 0;
    dec4 Fee = 0;
    
    dec4 QuantityLeft = 0;
    dec4 PriceUAH = 0;
};

FStockTrade ParseTrade(const FFileLine& Line);