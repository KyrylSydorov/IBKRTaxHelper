// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

enum class EProfitType
{
    Trade,
    Dividend,
    Bond,
    OtherAccrual,
};

enum class ETaxType
{
    IncomeTax,
    DividendTax,
    MilitaryTax,
};

struct FTaxToPay
{
    ETaxType TaxType = ETaxType::IncomeTax;
    dec4 TaxPercent = 0;
    dec4 Amount = 0;
};

string LexToStr(ETaxType TaxType);

using FTaxList = vector<FTaxToPay>;

ostream& operator<<(ostream& os, const FTaxToPay& TaxToPay);
ostream& operator<<(ostream& os, const FTaxList& TaxList);

class FTaxManager
{
public:
    static FTaxList CalculateTaxes(EProfitType ProfitType, dec4 Profit, int Year);
};
