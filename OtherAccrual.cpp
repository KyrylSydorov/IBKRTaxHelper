// Kyryl Sydorov, 2025

#include "OtherAccrual.h"

ostream& operator<<(ostream& os, const FOtherAccrual& Accrual)
{
    return os << "Accrual: " << Accrual.Description << " on " << Accrual.Date << ". Amount: " << Accrual.Amount << " " << Accrual.Currency << " (" << Accrual.AmountUAH << " UAH)\n";
}
