// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

struct FDividend
{
    string Symbol;
    FDate Date;
    string Currency;
    dec4 Amount = 0;
    dec4 AmountUAH = 0;
};

ostream& operator<<(ostream& os, const FDividend& Dividend);

FDividend ParseDividend(const FFileLine& Line);
