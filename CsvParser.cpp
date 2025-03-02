// Kyryl Sydorov, 2025

#include <fstream>

#include "CsvParser.h"

using std::ifstream;

vector<vector<string>> FCsvParser::ParseFile(const string& FileName)
{
    ifstream File(FileName);
    FFileLines Result;
    string Line;
    while (getline(File, Line))
    {
        FFileLine LineData;
        string CurrentValue;
        bool bInQuotes = false;
        for (size_t i = 0; i < Line.size(); ++i)
        {
            char c = Line[i];
            if (c == '\"')
            {
                if (bInQuotes && i + 1 < Line.size() && Line[i + 1] == '\"')
                {
                    CurrentValue.push_back('\"');
                    ++i;
                }
                else
                {
                    bInQuotes = !bInQuotes;
                }
            }
            else if (c == ',' && !bInQuotes)
            {
                LineData.push_back(CurrentValue);
                CurrentValue.clear();
            }
            else
            {
                CurrentValue.push_back(c);
            }
        }
        LineData.emplace_back(std::move(CurrentValue));
        Result.push_back(std::move(LineData));
    }
    return Result;
}

bool FCsvParser::StartsWith(const FFileLine& Line, const FFileLine& Prefix)
{
    for (size_t i = 0; i < Prefix.size(); ++i)
    {
        if (i >= Line.size() || Line[i] != Prefix[i])
        {
            return false;
        }
    }
    return true;
}
