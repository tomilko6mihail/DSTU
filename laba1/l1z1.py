def main():
    print("Введите число N: ")
    N = input()
    if N.isnumeric():
        if N.count(".") == 0:
            N = int(N)
            if N > 1:
                i = 0
                summ = 0
                while summ < N:
                    i += 1
                    summ += i
                print("K: ", i, " summa: ", summ)
        else:
            print("Некорректное число. ")
            main()
    else:
        print("Некорректное число. ")
        main()
    
main()
