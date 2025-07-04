// Kyryl Sydorov, 2025

#include "TaxManager.h"

string LexToStr(ETaxType TaxType)
{
    switch (TaxType)
    {
        case ETaxType::IncomeTax:
            return "Income Tax";
        case ETaxType::DividendTax:
            return "Dividend Tax";
        case ETaxType::MilitaryTax:
            return "Military Tax";
        default:
            return "Unknown Tax Type";
    }
}

ostream& operator<<(ostream& os, const FTaxToPay& TaxToPay)
{
    return os << LexToStr(TaxToPay.TaxType) << " (" << TaxToPay.TaxPercent << "%): " << TaxToPay.Amount << " UAH\n";
}

ostream& operator<<(ostream& os, const FTaxList& TaxList)
{
    for (const FTaxToPay& Tax : TaxList)
    {
        os << Tax;
    }
    return os;
}

vector<FTaxToPay> FTaxManager::CalculateTaxes(EProfitType ProfitType, dec4 Profit, int Year)
{
    using FTaxRules = unordered_map<EProfitType, FTaxList>; // Profit Type to Tax List
    using FTaxRulesCollection = unordered_map<int, FTaxRules>; // Year to Tax Rules
    
    static const FTaxRulesCollection TaxRulesCollection =
        {
            {
                2024, FTaxRules{
                    { EProfitType::Trade, { {ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 1.5} } },
                    { EProfitType::Dividend, {{ETaxType::DividendTax, 9}, {ETaxType::MilitaryTax, 1.5} } },
                    { EProfitType::Bond, {{ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 1.5} } },
                    { EProfitType::OtherAccrual, {{ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 1.5} } }
                }
            },
            {
                2025, FTaxRules{
                                    { EProfitType::Trade, { {ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 5} } },
                                    { EProfitType::Dividend, {{ETaxType::DividendTax, 9}, {ETaxType::MilitaryTax, 5} } },
                                    { EProfitType::Bond, {{ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 5} } },
                                    { EProfitType::OtherAccrual, {{ETaxType::IncomeTax, 18}, {ETaxType::MilitaryTax, 5} } }
                }
            },
        };

    FTaxRulesCollection::const_iterator TaxRules = TaxRulesCollection.find(Year);
    if (TaxRules == TaxRulesCollection.end())
    {
        cout << "No tax rules for year " << Year << ".\n";
        return {};
    }

    FTaxRules::const_iterator ProfitTypeRules = TaxRules->second.find(ProfitType);
    if (ProfitTypeRules == TaxRules->second.end())
    {
        cout << "No tax rules for profit type " << static_cast<int>(ProfitType) << " in year " << Year << ".\n";
        return {};
    }

    // Intentionally copy the tax list to avoid modifying the original rules
    FTaxList TaxList = ProfitTypeRules->second;

    for (FTaxToPay& Tax : TaxList)
    {
        Tax.Amount = Profit * (Tax.TaxPercent / 100);
    }

    return TaxList;
}
