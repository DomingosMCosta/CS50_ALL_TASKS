def get_int(prompt):
    while True:
        try:
            num = int(input(prompt))
            if num >= 0:
                return num
            else:
                pass
        except ValueError:
            pass


def main():
    num = get_int("Number: ")
    list_num = [int(digit) for digit in str(num)]
    even_sum = 0
    odd_sum = 0
    # len(list_num) number of digits
    for i in range(0, len(list_num[::-1]), 2):
        even_sum += list_num[::-1][i]

    for i in range(1, len(list_num[::-1]), 2):
        x2_digits = list_num[::-1][i] * 2
        for digit in str(x2_digits):
            odd_sum += int(digit)

    if (even_sum + odd_sum) % 10 == 0:
        if len(list_num) == 15 and list_num[0] == 3 and (list_num[1] == 4 or list_num[1] == 7):
            print("AMEX")
        if len(list_num) == 16 and list_num[0] == 5 and (list_num[1] == 1 or list_num[1] == 5):
            print("MASTERCARD")
        if (len(list_num) == 16 or len(list_num) == 13) and list_num[0] == 4:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


main()
