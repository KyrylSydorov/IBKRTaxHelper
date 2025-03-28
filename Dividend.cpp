// Kyryl Sydorov, 2025

#include "Dividend.h"

ostream& operator<<(ostream& os, const FDividend& Dividend)
{
    return os << "Dividend: " << Dividend.Symbol << " paid on " << Dividend.Date << ". Profit: " << Dividend.AmountUAH << '\n';
}

FDividend ParseDividend(const FFileLine& Line)
{
    FDividend Result;
    
    Result.Symbol = Line[4].substr(0, Line[4].find('('));
    ParseDateIBKR(Line[3], Result.Date);
    Result.Currency = Line[2];
    Result.Amount = std::stod(Line[5]);
    
    return Result;
}
