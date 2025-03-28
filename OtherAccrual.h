// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

struct FOtherAccrual
{
    string Currency;
    FDate Date;
    string Description;
    dec4 Amount = 0;
    dec4 AmountUAH = 0;
};

ostream& operator<<(ostream& os, const FOtherAccrual& Accrual);