// Kyryl Sydorov, 2025

#pragma once

#include <iostream>

using std::istream;
using std::ostream;

using int64 = long long;

// Decimal number with 4 digits after the decimal point
struct dec4
{
    dec4(int InValue = 0);
    dec4(double InValue);

    dec4& operator+=(dec4 Rhs);
    dec4& operator-=(dec4 Rhs);
    dec4& operator*=(dec4 Rhs);
    dec4& operator/=(dec4 Rhs);

    dec4 operator-();

    dec4& ToAbsolute();
    
    int64 Value;
};

dec4 operator+(dec4 Lhs, dec4 Rhs);
dec4 operator-(dec4 Lhs, dec4 Rhs);
dec4 operator*(dec4 Lhs, dec4 Rhs);
dec4 operator/(dec4 Lhs, dec4 Rhs);

bool operator==(dec4 Lhs, dec4 Rhs);
bool operator!=(dec4 Lhs, dec4 Rhs);
bool operator<(dec4 Lhs, dec4 Rhs);
bool operator>(dec4 Lhs, dec4 Rhs);
bool operator<=(dec4 Lhs, dec4 Rhs);
bool operator>=(dec4 Lhs, dec4 Rhs);

dec4 abs(dec4 Value);

std::ostream& operator<<(std::ostream& Out, dec4 Value);
std::istream& operator>>(std::istream& In, dec4& Value);
