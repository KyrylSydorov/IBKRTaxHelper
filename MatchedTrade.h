// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

struct FMatchedTrade
{
    string Symbol;
    dec4 Quantity = 0;
    FDate DateBought;
    dec4 PriceBought = 0;
    FDate DateSold;
    dec4 PriceSold = 0;

    dec4 GetSumBought() const;
    dec4 GetSumSold() const;
    dec4 GetProfit() const;
};

ostream& operator<<(ostream& os, const FMatchedTrade& Trade);
