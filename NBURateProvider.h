// Kyryl Sydorov, 2025

#pragma once

#include "RateProvider.h"

#if USE_NBU_API
/**
 * Rate provider that uses the National Bank of Ukraine API to get the exchange rates.
 */
class FNBURateProvider : public IRateProvider
{
public:
    FNBURateProvider();
    virtual ~FNBURateProvider() override;

    virtual dec4 GetRate(const FDate& Date, const string& Currency) const override;

private:
    void* Curl;

    // Cache to avoid multiple requests to the NBU API
    using FRates = unordered_map<string, dec4>;
    mutable unordered_map<FDate, unordered_map<string, dec4>> Rates;
};
#endif
