// Kyryl Sydorov, 2025

#pragma once

#include "DateTime.h"

#include "RateProvider.h"

/**
 * @brief Simple provider which takes rates from a CSV file
 */
class FRateStorage : public IRateProvider
{
public:
    FRateStorage(const string& FileName);

    virtual dec4 GetRate(const FDate& Date, const string& Currency) const override;

private:
    using FRates = unordered_map<string, dec4>;
    unordered_map<FDate, unordered_map<string, dec4>> Rates;
};
