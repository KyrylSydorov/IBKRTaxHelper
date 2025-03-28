// Kyryl Sydorov, 2025

#include "CommonIncludes.h"

#include "CsvParser.h"
#include "IBKRManager.h"
#include "RateProvider.h"

#if USE_NBU_API
#include "NBURateProvider.h"
#else
#include "RateStorage.h"
#endif

int main()
{
    unique_ptr<IRateProvider> RateProvider;
#if USE_NBU_API
    RateProvider = make_unique<FNBURateProvider>();
#else
    static const string RatesFileName = "Rates.csv";
    RateProvider = make_unique<FRateStorage>(RatesFileName);
#endif
    
    const FFileLines Lines = FCsvParser::ParseFolder("ActivityStatements");
    FIBKRManager IBKRManager(std::move(RateProvider));

    IBKRManager.ParseStatement(Lines);
    
    IBKRManager.PrintEverything();
}