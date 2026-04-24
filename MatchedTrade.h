// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

struct FMatchedTrade
{
    string Symbol;
    dec4 Quantity = 0;
    FDate DateBought;
    dec4 PriceBoughtUAH = 0;
    FDate DateSold;
    dec4 PriceSoldUAH = 0;

    string Currency;
    dec4 PriceBought = 0; // In original currency
    dec4 PriceSold = 0; // In original currency

    dec4 GetSumBoughtUAH() const;
    dec4 GetSumSoldUAH() const;
    dec4 GetProfitUAH() const;

	dec4 GetSumBought() const;
	dec4 GetSumSold() const;
	dec4 GetProfit() const;

    static void PrintHeader(ostream& os);
};

ostream& operator<<(ostream& os, const FMatchedTrade& Trade);
