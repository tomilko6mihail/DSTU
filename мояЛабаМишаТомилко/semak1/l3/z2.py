from random import randint

def f():
    try:
        N = int(input("Количество строк: "))
        M = int(input("Количество столбцов: "))
        if N < 1 or M < 1:
            40/0
        else:
            li = None
            li = [[randint(0, 9) for j in range(M)] for i in range(N)] #генерация матрицы

            ##красивый вывод таблицы
            for i in range(N):
                for j in range(M):
                    print(str(li[i][j]) + "   ", end='')
                print(sep="/")

            localMax = 0
            idN = 0
            idM = 0
            diction = {}
            for i in range(N): # ищем максимальный элемент в строке
                for j in range(M):
                    if li[i][j] >= localMax:
                        localMax = li[i][j]
                        idM = j
                        idN = i
                diction[i] = [localMax, idN, idM] #заносим максимальный элемент с его номером в строке и столбце в словарь по ключу строки
                localMax = 0 #обнуляем счетчик для следующей строки
            print(diction)

            globalMin = 10
            curIdx = 0
            for i in range(N): # ищем минимальный элемент среди полученных максимальных значений строк
                if diction[i][0] <= globalMin:
                    globalMin = diction[i][0] #для корректного высчитывания минимального значения обновляем переменную
                    curIdx = i # находим айдишник строки  с минимальным элементом среди всех строк
                    
            print("Минимальный эл-нт среди макс. эл-ов: ",  diction[curIdx][0]) #по айдишнику выводим из словаря нужный элемент с номером строки и столбца
            print("Его строка: ", diction[curIdx][1] + 1, ", столбец: ", diction[curIdx][2] + 1)
            return 0
    except:
        print("Неверно введены значения.")
        print("")
        f()
f()