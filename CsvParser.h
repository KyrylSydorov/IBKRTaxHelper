﻿// Kyryl Sydorov, 2025

#pragma once

#include "CommonIncludes.h"

struct FCsvParser
{
    static FFileLines ParseFolder(const string& FolderName);
    
    // Contains lines of the file as vectors of strings
    static FFileLines ParseFile(const string& FileName);

    static bool StartsWith(const FFileLine& Line, const FFileLine& Prefix);
};
