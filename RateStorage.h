// Kyryl Sydorov, 2025

#pragma once

#include "DateTime.h"

class FRateStorage
{
public:
    FRateStorage(const string& FileName);

    dec4 GetRate(const FDate& Date, const string& Currency) const;

private:
    using FRates = unordered_map<string, dec4>;
    unordered_map<FDate, unordered_map<string, dec4>> Rates;
};
