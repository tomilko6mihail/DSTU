import random                                                                       # импортируем рандом для работы с рандомом

try:                                                                                # если ошибок нет, то выполняем этот код
    def returnModule(x):                                                            # функция, возвращающая модуль числа
        if x < 0:
            return x * -1
        else:
            return x
        
    summa = 0
    proizvedenie = 1                                                                # необходимо, чтобы выполнялось proizvedenie *= li[i] 
    maxModule = 0                                                                   # максимальный модуль минимален, чтоб сравнивать его с текущим числом и найти реальный больший ммодуль
    minModule = 10                                                                  # минимальный модуль максимален, чтоб сравнивать его с текущим числом и найти реальный меньший ммодуль
    li = []
    n = int(input("Введите размер списка: "))
    idMin = 0
    idMax = 0
    if n > 0:
        for i in range(n):                                                              # заполняем список рандомными числами (по условию)
            li += [random.randint(-10, 10)]                                             # в диапазоне от -10 до 10

        print("Полученный список: ", li)
        for i in range(len(li)):                                                        # пробегаемся по списку 

            if li[i] > 0: summa += li[i]                                                # если нашли положительный элемент, то прибавляем к сумме
            if returnModule(li[i]) > maxModule: 
                maxModule = returnModule(li[i])     # если модуль текщего числа меньше минимального модуля, то перезаписываем минимальный модуль
                idMax = i
            if returnModule(li[i]) < minModule:
                minModule = returnModule(li[i]) 
                idMin = i
        li2 = []

        for i in range(len(li)):
            if (i > idMax or i > idMin) and (i < idMax or i < idMin):
                li2 += [li[i]]
        print(li2)
        if len(li2) > 0:
            for i in range(len(li2)):
                proizvedenie *= li2[i]
        # for i in range(1, len(li)):                                                        # снова пробегаемся по списку, уже имея нужные данные
        #     if li[i - 1] == returnModule(li[i - 1]) == maxModule or li[i - 1] == returnModule(li[i - 1]) == minModule:
        #         proizvedenie *= li[i]

        
        print("Сумма: ", summa, "произведение: ", proizvedenie)
    else:
        print("Доступен ввод только целочисленных значений.")
except:
    print("Доступен ввод только целочисленных значений.")