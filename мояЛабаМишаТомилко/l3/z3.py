from random import randint

def f():
    try:
        countstr = int(input("Введите длину строки: "))
        if countstr < 1:
            40/0
        else:
            
            strr = [randint(0, 9) for i in range(countstr)] #генерация строки
            diction = {}
            print(strr)
            for i in range(10): #заполнение словаря повторениями в строке
                diction[i] = strr.count(i)
            needInt = int(input("Введите интересующее число: "))
            if needInt < 0: 
                40/0
            else:
                print("Количество ", needInt, " в строке: ", diction[needInt]) #вывод по ключу
            return 0
    except:
        print("Неверно введены значения.")
        print("")
        f()
f()