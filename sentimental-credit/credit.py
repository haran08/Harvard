def main():
    while True:
        number = input("Number: ")

        if not number.isdigit():
            continue
        break

    number = int(number)

    if number < 0:
        print("INVALID")
        return

    isValid = CardNumberValidation(number)

    if isValid and MasterCardValidation(number):
        print("MASTERCARD")
        return
    elif isValid and AmericanExpressCardValidation(number):
        print("AMEX")
        return
    elif isValid and VisaCardValidation(number):
        print("VISA")
        return
    else:
        print("INVALID")
        return


# Function for validate card numbers
def CardNumberValidation(cardNumber: int) -> bool:
    decimal = 0
    i = 1
    secondResult = 0
    while decimal < cardNumber:
        fix = 10 * i
        decimal = 100 * i
        secondDigit = cardNumber % decimal // fix * 2
        i *= 100

        secondResult += secondDigit // 10 + secondDigit % 10

    i = 1
    decimal = 0
    sum = secondResult
    while decimal < cardNumber:
        fix = 1 * i
        decimal = 10 * i
        digit = cardNumber % decimal // fix
        i *= 100

        sum += digit

    if sum % 10 == 0:
        return True
    return False

# Differents companies cards validation functions
# Start


def MasterCardValidation(card: int) -> bool:
    CSNu1 = 51
    CSNu2 = 52
    CSNu3 = 53
    CSNu4 = 54
    CSNu5 = 55

    Sn = card // pow(10, 14)

    return len(str(card)) == 16 and (
        Sn == CSNu1 or Sn == CSNu2 or
        Sn == CSNu3 or Sn == CSNu4 or
        Sn == CSNu5
    )


def AmericanExpressCardValidation(card: int) -> bool:
    CSNu1 = 34
    CSNu2 = 37

    Sn = card // pow(10, 13)

    return len(str(card)) == 15 and (
        Sn == CSNu1 or Sn == CSNu2
    )


def VisaCardValidation(card: int) -> bool:
    CSNu1 = 4
    is16H = len(str(card)) == 16
    is13H = len(str(card)) == 13
    Sn = int()

    if is16H:
        Sn = card // pow(10, 15)
    elif is13H:
        Sn = card // pow(10, 12)

    return (is16H or is13H) and Sn == CSNu1
# End


main()
