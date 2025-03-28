// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "Dividend.h"
#include "MatchedTrade.h"
#include "OtherAccrual.h"
#include "StockTrade.h"

struct IRateProvider;

/**
 * Manager for the Interactive Brokers parsing and data processing.
 */
class FIBKRManager
{
public:
    FIBKRManager(unique_ptr<IRateProvider> RateProvider);

    bool ParseStatement(const FFileLines& Lines);

    void PrintEverything() const;

    void PrintMatchedTrades() const;
    void PrintDividends() const;
    void PrintOtherAccruals() const;
    
private:
    bool TryProcessTrade(const FFileLine& Line);
    bool TryProcessDividend(const FFileLine& Line);
    bool TryProcessSYEP(const FFileLine& Line);

    void PrintMatchedTradesByYear(int Year) const;
    void PrintDividendsByYear(int Year) const;
    void PrintOtherAccrualsByYear(int Year) const;
    
    unique_ptr<IRateProvider> RateProvider;

    vector<FStockTrade> Trades;
    map<int, vector<FMatchedTrade>> MatchedTradesByYear;

    map<int, vector<FDividend>> DividendsByYear;

    map<int, vector<FOtherAccrual>> OtherAccrualsByYear;

    int MinYear = INT_MAX;
    int MaxYear = 0;
};
