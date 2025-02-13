def main():
    print("Введите процент p (от 1 до 24): ")
    p = input()
    if p.isnumeric():
        p = float(p)
        if p > 0 and p < 25:
            i = 1
            s = 1000
            while s <= 1100:
                s += s*(p/100)
                i += 1
            print("Количество месяцев - ", i, "итоговый вклад - ", s)
        else:
            print("Некорректное число. ")
            main() 
    else:
        print("Некорректное число. ")
        main()
main()