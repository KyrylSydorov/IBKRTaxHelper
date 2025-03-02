// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

struct FDate
{
    int Day = 0;
    int Month = 0;
    int Year = 0;

    bool operator==(const FDate& Other) const
    {
        return Day == Other.Day && Month == Other.Month && Year == Other.Year;
    }
};

// Provide a specialization of std::hash for MyKey
template <>
struct std::hash<FDate>
{
    std::size_t operator()(const FDate& Date) const noexcept {
        const std::size_t Hash1 = std::hash<int>{}(Date.Day);
        const std::size_t Hash2 = std::hash<int>{}(Date.Month);
        const std::size_t Hash3 = std::hash<int>{}(Date.Year);
        return Hash1 ^ (Hash2 << 1) ^ (Hash3 << 2);
    }
};

// Date format: "YYYY-MM-DD"
constexpr int DateLen = 10;

void ParseDateIBKR(const string& DateStr, FDate& Date);
void ParseDateNBU(const string& DateStr, FDate& Date);

ostream& operator<<(ostream& Out, const FDate& Date);