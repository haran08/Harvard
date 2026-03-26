#include <cs50.h>
#include <stdio.h>

bool CardNumberValidation(long cardNumber);

long power(int base, int power);

bool NumbersQuantityCheck(long card, int n);

bool MasterCardValidation(long card);

bool AmericanExpressCardValidation(long card);

bool VisaCardValidation(long card);

int main(void)
{
    long card = get_long("Number: ");
    if (card < 0)
    {
        printf("INVALID");
        return 0;
    }

    bool valid = CardNumberValidation(card);
    if (valid && MasterCardValidation(card))
    {
        printf("MASTERCARD\n");
        return 0;
    }
    else if (valid && AmericanExpressCardValidation(card))
    {
        printf("AMEX\n");
        return 0;
    }
    else if (valid && VisaCardValidation(card))
    {
        printf("VISA\n");
        return 0;
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}

long power(int base, int power)
{
    long result = base;
    for (int i = 1; i < power; i++)
    {
        result = result * base;
    }
    return result;
}

bool MasterCardValidation(long card)
{
    const int CorrectStartNumber1 = 51;
    const int CorrectStartNumber2 = 52;
    const int CorrectStartNumber3 = 53;
    const int CorrectStartNumber4 = 54;
    const int CorrectStartNumber5 = 55;

    int startNumber = card / power(10, 14);

    return NumbersQuantityCheck(card, 16) &&
           (startNumber == CorrectStartNumber1 || startNumber == CorrectStartNumber2 ||
            startNumber == CorrectStartNumber3 || startNumber == CorrectStartNumber4 ||
            startNumber == CorrectStartNumber5);
}

bool AmericanExpressCardValidation(long card)
{
    const int CorrectStartNumber1 = 34;
    const int CorrectStartNumber2 = 37;

    int startNumber = card / power(10, 13);

    return NumbersQuantityCheck(card, 15) &&
           (startNumber == CorrectStartNumber1 || startNumber == CorrectStartNumber2);
}

bool VisaCardValidation(long card)
{
    const int CorrectStartNumber1 = 4;
    int startNumber = 0;
    bool numbers16Check = NumbersQuantityCheck(card, 16);
    bool numbers13Check = NumbersQuantityCheck(card, 13);

    if (numbers16Check)
    {
        startNumber = card / power(10, 15);
    }
    else if (numbers13Check)
    {
        startNumber = card / power(10, 12);
    }

    return (numbers13Check || numbers16Check) && startNumber == CorrectStartNumber1;
}

bool NumbersQuantityCheck(long card, int n)
{
    int decimalHouses = 0;
    long number = card;
    for (int i = 1; i <= n; i++)
    {
        decimalHouses = i;
        number = number / 10;
        if (number == 0)
        {
            break;
        }
    }
    return n == decimalHouses;
}

bool CardNumberValidation(long cardNumber)
{
    long decimal = 0;
    long fix = 0;
    long i = 1;
    int secondResult = 0;
    int secondDigit;
    int digit;
    while (decimal < cardNumber)
    {
        fix = 10 * i;
        decimal = 100 * i;
        secondDigit = cardNumber % decimal / fix * 2;

        secondResult += secondDigit / 10 + secondDigit % 10;
        i *= 100;
    }

    i = 1;
    decimal = 0;
    int sum = secondResult;
    while (decimal < cardNumber)
    {
        fix = 1 * i;
        decimal = 10 * i;
        digit = cardNumber % decimal / fix;

        sum += digit;
        i *= 100;
    }

    if (sum % 10 == 0)
    {
        return true;
    }
    return false;
}
