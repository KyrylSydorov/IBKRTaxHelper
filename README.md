Simple program which might help with tax calculations for trades on Interactive Brokers.
I have created it for myself in 3 hours so it might contain bugs.
UPD: spent 2 more hours to implement the API version

**ActivityStatements**
Create report as the annual (or Year-To-Date) csv Activity report on your IBKR reports page.
Put all the reports into ActivityStatements. They must be sorted lexicographically (so the "older" annual report is processed earlier)

**National Bank of Ukraine API**
Current version of this app supports retrieving the rates via official API. Due to some bugs on NBU side, the application works a lot slower since we have to wait for responses from server.
Optionally, you can export the csv and import it to the project manually and change the Public Definition USE_NBU_API to 0

**Libcurl**
In order to use API version you have to install libcurl via vcpkg.

**Rates.csv**
This step is optional
Export the csv information with currency exchange rates from https://bank.gov.ua/ua/markets/exchangerate-chart?cn%5B%5D=USD&cn%5B%5D=EUR&cn%5B%5D=PLN&startDate=01.01.2024&endDate=31.12.2024
