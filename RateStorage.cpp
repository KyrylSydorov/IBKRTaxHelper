// Kyryl Sydorov, 2025

#include "RateStorage.h"
#include "CsvParser.h"

FRateStorage::FRateStorage(const string& FileName)
{
    FFileLines Lines = FCsvParser::ParseFile(FileName);

    bool bHeaderRead = false;
    
    for (const FFileLine& Line : Lines)
    {
        if (!bHeaderRead)
        {
            // Skip first line
            bHeaderRead = true;
            continue;
        }
        
        if (Line.size() != 7)
        {
            continue;
        }

        // Sample:
        // [0] - 31.12.2024,
        // [1] - 00.00,
        // [2] - 978,
        // [3] - EUR,
        // [4] - 1,
        // [5] - Євро,
        // [6] - 43.9266

        FDate Date;
        ParseDateNBU(Line[0], Date);

        FRates& DayRates = Rates[Date];

        DayRates[Line[3]] = std::stod(Line[6]);
    }
}

dec4 FRateStorage::GetRate(const FDate& Date, const string& Currency) const
{
    return Rates.at(Date).at(Currency);
}
