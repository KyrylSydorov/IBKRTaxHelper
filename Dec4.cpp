// Kyryl Sydorov, 2025

#include "Dec4.h"

#include <cmath>
#include <string>

dec4::dec4(int InValue)
    : Value(InValue * 10000)
{
}

dec4::dec4(double InValue)
    : Value(static_cast<int64>(rint(InValue * 10000)))
{
}

dec4& dec4::operator+=(dec4 Rhs)
{
    Value += Rhs.Value;
    return *this;
}

dec4& dec4::operator-=(dec4 Rhs)
{
    Value -= Rhs.Value;
    return *this;
}

dec4& dec4::operator*=(dec4 Rhs)
{
    Value = static_cast<int64>(rint(Value * Rhs.Value / 10000));
    return *this;
}

dec4& dec4::operator/=(dec4 Rhs)
{
    Value = static_cast<int64>(rint(Value * 10000 / Rhs.Value));
    return *this;
}

dec4 dec4::operator-()
{
    dec4 Copy(*this);
    Copy.Value = -Copy.Value;
    return Copy;
}

dec4& dec4::ToAbsolute()
{
    Value = std::abs(Value);
    return *this;
}

dec4 operator+(dec4 Lhs, dec4 Rhs)
{
    return Lhs += Rhs;
}

dec4 operator-(dec4 Lhs, dec4 Rhs)
{
    return Lhs -= Rhs;
}

dec4 operator*(dec4 Lhs, dec4 Rhs)
{
    return Lhs *= Rhs;
}

dec4 operator/(dec4 Lhs, dec4 Rhs)
{
    return Lhs /= Rhs;
}

bool operator==(dec4 Lhs, dec4 Rhs)
{
    return Lhs.Value == Rhs.Value;
}

bool operator!=(dec4 Lhs, dec4 Rhs)
{
    return !(Lhs == Rhs);
}

bool operator<(dec4 Lhs, dec4 Rhs)
{
    return Lhs.Value < Rhs.Value;
}

bool operator>(dec4 Lhs, dec4 Rhs)
{
    return Lhs.Value > Rhs.Value;
}

bool operator<=(dec4 Lhs, dec4 Rhs)
{
    return !(Lhs > Rhs);
}

bool operator>=(dec4 Lhs, dec4 Rhs)
{
    return !(Lhs < Rhs);
}

dec4 abs(dec4 Value)
{
    return Value.ToAbsolute();
}

std::ostream& operator<<(std::ostream& Out, dec4 Value)
{
    if (Value.Value < 0)
    {
        Out << '-';
        Value = -Value;
    }
    
    std::string s = std::to_string(Value.Value);
    if (s.size() < 5)
    {
        Out << "0.";
        for (size_t i = 0; i < 4 - s.size(); ++i)
        {
            Out << '0';
        }
    }
    else
    {
        Out << s.substr(0, s.size() - 4) << '.';
    }
    return Out << s.substr(s.size() - 4);
}

std::istream& operator>>(std::istream& In, dec4& Value)
{
    double Temp;
    In >> Temp;
    Value = Temp;
    return In;
}
