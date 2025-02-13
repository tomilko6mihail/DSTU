import math
def main():
    print("Введите число x: ")
    x = input()
    print("Введите число n: ")
    n = input()
    if x.isnumeric() and n.isnumeric():
        if x.count(".") == 0 and n.count(".") == 0:
            x = int(x)
            n = int(n)
            summ = 0
            for i in range(n): #поскольку начинаем с нуля, а должны с 1, т.е. i + 1
                summ += (x**(i+1)/math.factorial(i+1))
            print(summ)
        else:
            print("Некорректное число. ")
            main()
    else:
        print("Некорректное число. ")
        main()
main()