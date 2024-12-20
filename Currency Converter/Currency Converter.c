#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CURRENCY_COUNT 5
#define FEE_RATE 0.01

typedef struct {
    char currencyName[50];
    char currencyCode[4];
    float exchangeRate;
} Currency;

typedef struct {
    char firstName[30];
    char lastName[30];
} User;

/*
convert_from - currency to convert from
convert_to - currency to convert to
amount is first multiplied by the exchange rate of the currency to convert from, and then divided
by the exchange rate of the currency to convert to, giving the final amount.
*/
float convert(float amount, Currency* convert_from, Currency* convert_to) {
    amount = amount * convert_from->exchangeRate;
    amount = amount / convert_to->exchangeRate;
    return amount;
}

int main() {
    // Variable initialization
    char fromCode[4], toCode[4], repeat[4], confirmation[4], showTransactions[4];
    char c;
    float amount;
    int transactionCount = 1;
    User user;

    // Dynamic allocation of the currency array
    Currency* currencyArray = malloc(CURRENCY_COUNT * sizeof(Currency));

    if (currencyArray == NULL) {
        printf("Memory allocation error");
        return 1;
    }

    // Initialization of array elements
    strcpy(currencyArray[0].currencyName, "US Dollar");
    strcpy(currencyArray[0].currencyCode, "USD");
    currencyArray[0].exchangeRate = 107.3081;

    strcpy(currencyArray[1].currencyName, "Euro");
    strcpy(currencyArray[1].currencyCode, "EUR");
    currencyArray[1].exchangeRate = 117.2805;

    strcpy(currencyArray[2].currencyName, "Swiss Franc");
    strcpy(currencyArray[2].currencyCode, "CHF");
    currencyArray[2].exchangeRate = 119.1216;

    strcpy(currencyArray[3].currencyName, "Russian Ruble");
    strcpy(currencyArray[3].currencyCode, "RUB");
    currencyArray[3].exchangeRate = 1.3092;

    strcpy(currencyArray[4].currencyName, "Serbian Dinar");
    strcpy(currencyArray[4].currencyCode, "RSD");
    currencyArray[4].exchangeRate = 1;

    printf("\nExchange rates relative to Serbian Dinar, obtained from the official website of the National Bank of Serbia\n");
    for (int i = 0; i < CURRENCY_COUNT; i++) {
        printf("\nCurrency Name: %s\nCurrency Code: %s\nExchange Rate: %.4f\n",
            currencyArray[i].currencyName, currencyArray[i].currencyCode, currencyArray[i].exchangeRate);
    }
    printf("\nFEE RATE IS 1%%\n\n");

    printf("Enter your first name: ");
    gets(user.firstName);

    printf("Enter your last name: ");
    gets(user.lastName);

    printf("\n");

    FILE* file;
    file = fopen("transactions.txt", "w");

    do {
        printf("Enter the currency code to convert from: ");
        scanf("%s", fromCode);

        printf("Enter the amount to convert: ");
        scanf("%f", &amount);

        printf("Enter the currency code to convert to: ");
        scanf("%s", toCode);

        // Initialize pointers to NULL
        Currency* fromCurrency = NULL;
        Currency* toCurrency = NULL;

        // Find the currencies in the array
        for (int i = 0; i < CURRENCY_COUNT; i++) {
            if (strcmp(currencyArray[i].currencyCode, fromCode) == 0) {
                fromCurrency = &currencyArray[i];
            }
            else if (strcmp(currencyArray[i].currencyCode, toCode) == 0) {
                toCurrency = &currencyArray[i];
            }
        }

        // Handle same currency case
        if (fromCurrency != NULL && toCurrency == NULL) {
            printf("You cannot exchange the same currency, please re-enter the currency code to convert to\n");
            printf("Enter the currency code to convert to: ");
            scanf("%s", toCode);

            for (int i = 0; i < CURRENCY_COUNT; i++) {
                if (strcmp(currencyArray[i].currencyCode, toCode) == 0) {
                    toCurrency = &currencyArray[i];
                }
            }
        }

        // Check if either currency is NULL
        if (fromCurrency == NULL || toCurrency == NULL) {
            printf("Invalid currency codes\n");
            return 1;
        }

        float convertedAmount = convert(amount, fromCurrency, toCurrency);
        printf("%.2f %s = %.2f %s\n", amount, fromCurrency->currencyCode, convertedAmount, toCurrency->currencyCode);

        float fee = convertedAmount * FEE_RATE;
        float finalAmount = convertedAmount - fee;
        printf("After deducting the fee, you receive: %.2f %s\n", finalAmount, toCurrency->currencyCode);

        printf("\nDo you want to confirm the transaction?\n");
        scanf("%s", confirmation);
        if (strcmp(confirmation, "YES") == 0 || strcmp(confirmation, "yes") == 0) {
            fprintf(file, "Transaction Number: %d\n", transactionCount);
            fprintf(file, "Name: %s, Surname: %s\n", user.firstName, user.lastName);
            fprintf(file, "Amount in %s to convert: %.2f\n", fromCode, amount);
            fprintf(file, "Converted amount in %s: %.2f\n", toCode, finalAmount);
            fprintf(file, "Fee (1%%): %.2f %s\n\n", fee, toCode);
            transactionCount++;
        }

        printf("\nDo you want to exchange money again?\n");
        scanf("%s", repeat);

        printf("\n");
    } while (strcmp(repeat, "YES") == 0 || strcmp(repeat, "yes") == 0);

    fclose(file);

    printf("\nDo you want to view all transactions? ");
    scanf("%s", showTransactions);

    printf("\n");
    if (strcmp(showTransactions, "YES") == 0 || strcmp(showTransactions, "yes") == 0) {
        file = fopen("transactions.txt", "r");

        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        while ((c = fgetc(file)) != EOF) {
            printf("%c", c);
        }
    }

    free(currencyArray);
    fclose(file);
    return 0;
}
