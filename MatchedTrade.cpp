// Kyryl Sydorov, 2025

#include "MatchedTrade.h"

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

ostream& operator<<(ostream& os, const FMatchedTrade& Trade)
{
    return os
        << "Trade: " << Trade.Symbol << '*' << Trade.Quantity << ' '
        << "\t Date Bought: " << Trade.DateBought << ' '
        << "\t Price Bought: " << Trade.PriceBought << ' '
        << "\t Sum Bought: " << Trade.GetSumBought() << ' '
        << "\t Date Sold: " << Trade.DateSold << ' '
        << "\t Price Sold: " << Trade.PriceSold << ' '
        << "\t Sum Sold: " << Trade.GetSumSold() << ' '
        << "\t Profit: " << Trade.GetProfit() << '\n';
}
