// Kyryl Sydorov, 2025

#include "NBURateProvider.h"

#if USE_NBU_API
#include <curl/curl.h>

namespace NBURateProvider::Internal
{
    size_t WriteCallback(void* Contents, size_t Size, size_t MemBlocks, void* User) {
        string* String = static_cast<string*>(User);
        const size_t TotalSize = Size * MemBlocks;
        String->append(static_cast<char*>(Contents), TotalSize);
        return TotalSize;
    }
}

FNBURateProvider::FNBURateProvider()
{
    Curl = curl_easy_init();
    if (!Curl)
    {
        std::cerr << "curl_easy_init() failed" << endl;
    }
}

FNBURateProvider::~FNBURateProvider()
{
    curl_easy_cleanup(Curl);
    Curl = nullptr;
}

dec4 FNBURateProvider::GetRate(const FDate& Date, const string& Currency) const
{
    FRates& DayRates = Rates[Date];
    const auto FoundRate = DayRates.find(Currency);
    if (FoundRate != DayRates.end())
    {
        cout << "Rate for " << Currency << " on " << Date.ToStringNoDelim() << " is cached" << endl;
        return FoundRate->second;
    }
    
    if (!Curl)
    {
        cout << "Curl is not initialized" << endl;
        return 0.0;
    }
        
    string Buffer;

    // There is a bug in the NBU API, it returns 500 Internal Server Error for some dates if I specify the valcode
    //const string URL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=" + Currency
    //    + "&date=" + Date.ToStringNoDelim() + "&json";

    const string URL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?date=" + Date.ToStringNoDelim() + "&json";
        
    curl_easy_setopt(Curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, &NBURateProvider::Internal::WriteCallback);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &Buffer);
    
    CURLcode Result;
    Result = curl_easy_perform(Curl);
    if (Result != CURLE_OK)
    {
        cout << "curl_easy_perform() failed: " << curl_easy_strerror(Result) << endl;
        return 0.0;
    }

    const size_t ErrorPos = Buffer.find("500 Internal Server Error");
    if (ErrorPos != string::npos)
    {
        cout << "500 Internal Server Error" << endl;
        return 0.0;
    }
        
    // #TODO Parse JSON normally instead of hardcoding
    const size_t CurrencyNamePos = Buffer.find(Currency);
    const size_t RatePos = Buffer.find_last_of("rate", CurrencyNamePos) + 3;
    const size_t RateEndPos = Buffer.find(',', RatePos);
    const string RateStr = Buffer.substr(RatePos, RateEndPos - RatePos);

    istringstream In(RateStr);
    dec4 Rate;
    In >> Rate;

    DayRates[Currency] = Rate;
    
    return Rate;
}
#endif
