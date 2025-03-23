// Kyryl Sydorov, 2025

#include "DateTime.h"

string FDate::ToStringNoDelim() const
{
    string Result = to_string(Year);
    if (Month < 10)
    {
        Result += '0';
    }
    Result += to_string(Month);
    if (Day < 10)
    {
        Result += '0';
    }
    Result += to_string(Day);
    return Result;
}

void ParseDateIBKR(const string& DateStr, FDate& Date)
{
    istringstream In(DateStr);
    
    char delim1, delim2, delim3, delim4, delim5;
    // Expect format: YYYY-MM-DD, HH:MM:SS
    if (!(In >> Date.Year >> delim1 >> Date.Month >> delim2 >> Date.Day))
    {
        return;
    }
}

void ParseDateNBU(const string& DateStr, FDate& Date)
{
    istringstream In(DateStr);
    
    char delim1, delim2;
    // Expect format: DD.MM.YYYY
    if (!(In >> Date.Day >> delim1 >> Date.Month >> delim2 >> Date.Year))
    {
        return;
    }
}

ostream& operator<<(ostream& Out, const FDate& Date)
{
    Out << Date.Day << "." << Date.Month << "." << Date.Year;
    return Out;
}
