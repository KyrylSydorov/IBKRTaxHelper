// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

#include "DateTime.h"

struct IRateProvider
{
    virtual ~IRateProvider() = default;
    
    virtual dec4 GetRate(const FDate& Date, const string& Currency) const = 0;
};
