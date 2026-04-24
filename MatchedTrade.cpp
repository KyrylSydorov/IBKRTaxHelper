// Kyryl Sydorov, 2025

#include <iomanip>

#include "MatchedTrade.h"

using namespace std;

dec4 FMatchedTrade::GetSumBoughtUAH() const
{
    return PriceBoughtUAH * Quantity;
}

dec4 FMatchedTrade::GetSumSoldUAH() const
{
    return PriceSoldUAH * Quantity;
}

dec4 FMatchedTrade::GetProfitUAH() const
{
    return GetSumSoldUAH() - GetSumBoughtUAH();
}

dec4 FMatchedTrade::GetSumBought() const
{
	return PriceBought * Quantity;
}

dec4 FMatchedTrade::GetSumSold() const
{
	return PriceSold * Quantity;
}

dec4 FMatchedTrade::GetProfit() const
{
	return GetSumSold() - GetSumBought();
}

static constexpr int SymbolNameLen = 4;
static constexpr int QuantityLen = 8;
static constexpr int DateStrLen = 10;
static constexpr int MoneyLen = 11;
static constexpr int MoneyCurrencyLen = 15;

void FMatchedTrade::PrintHeader(ostream& os)
{
    os << '\t' << setw(SymbolNameLen) << "Sym " << " | ";
    os << setw(QuantityLen) << "Quantity" << " | ";
    os << setw(2 * (MoneyLen + MoneyCurrencyLen) + DateStrLen + 12) << "Bought" << " | ";
    os << setw(2 * (MoneyLen + MoneyCurrencyLen) + DateStrLen + 12) << "Sold" << " | ";
    os << setw(MoneyLen + MoneyCurrencyLen + 3) << "Profit" << " |\n\t";

	os << setw(SymbolNameLen) << " " << " | ";
	os << setw(QuantityLen) << " " << " | ";
	os << setw(DateStrLen) << "Date" << " | ";
	os << setw(MoneyLen) << "Price UAH" << " | ";
	os << setw(MoneyLen) << "Sum UAH" << " | ";
	os << setw(MoneyCurrencyLen) << "Price" << " | ";
	os << setw(MoneyCurrencyLen) << "Sum" << " | ";
	os << setw(DateStrLen) << "Date" << " | ";
	os << setw(MoneyLen) << "Price UAH" << " | ";
	os << setw(MoneyLen) << "Sum UAH" << " | ";
	os << setw(MoneyCurrencyLen) << "Price" << " | ";
	os << setw(MoneyCurrencyLen) << "Sum" << " | ";
    os << setw(MoneyLen) << "UAH" << " | ";
    os << setw(MoneyCurrencyLen) << " " << " |\n\t";

    os << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

ostream& operator<<(ostream& os, const FMatchedTrade& Trade)
{
    ios OldFormat(nullptr);
    OldFormat.copyfmt(os);

    os << left  << setw(SymbolNameLen) << Trade.Symbol                                  << " | ";
    os << right << setw(QuantityLen)   << Trade.Quantity                                << " | ";
    os << right << setw(DateStrLen)    << Trade.DateBought                              << " | ";
    os << right << setw(MoneyLen)      << Trade.PriceBoughtUAH                          << " | ";
    os << right << setw(MoneyLen)      << Trade.GetSumBoughtUAH()                       << " | ";
    os << right << setw(MoneyLen)      << Trade.PriceBought    << ' ' << Trade.Currency << " | ";
    os << right << setw(MoneyLen)      << Trade.GetSumBought() << ' ' << Trade.Currency << " | ";
    os << right << setw(DateStrLen)    << Trade.DateSold                                << " | ";
    os << right << setw(MoneyLen)      << Trade.PriceSoldUAH                            << " | ";
    os << right << setw(MoneyLen)      << Trade.GetSumSoldUAH()                         << " | ";
    os << right << setw(MoneyLen)      << Trade.PriceSold      << ' ' << Trade.Currency << " | ";
    os << right << setw(MoneyLen)      << Trade.GetSumSold()   << ' ' << Trade.Currency << " | ";
	os << right << setw(MoneyLen)      << Trade.GetProfitUAH()                          << " | ";
	os << right << setw(MoneyLen)      << Trade.GetProfit()    << ' ' << Trade.Currency << " |\n";

    os.copyfmt(OldFormat);

    return os;
}
