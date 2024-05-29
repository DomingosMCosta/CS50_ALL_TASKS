def main():
    num = get_int("Height: ")
    counter = num - 1
    start = 1
    for i in range(num):
        print(" " * counter + "#" * start + "  " + "#" * start)
        counter -= 1
        start += 1
        '''alternative
        for _ in range(counter):
            #print(" ", end="")
        for _ in range(start):
            print("#", end="")
        print("  ", end="")
        for _ in range(start):
            print("#", end="")
        print()
        counter -= 1
        start += 1'''


def get_int(prompt):
    while True:
        try:
            num = int(input(prompt))
            if num > 0 and num <= 8:
                return num
            else:
                pass
        except ValueError:
            pass


main()
