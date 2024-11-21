from random import randint

def f():
    try:
        N = int(input("Количество строк: "))
        M = int(input("Количество столбцов: "))
        if N < 1 or M < 1:
            40/0
        else:
            li = None
            li = [[randint(0, 9) for j in range(M)] for i in range(N)]

            ##красивый вывод таблицы
            for i in range(N):
                for j in range(M):
                    print(str(li[i][j]) + "   ", end='')
                print(sep="/")

            maxsum = 0
            localsum = 0
            curIndex = 0
            for i in range(M): #сначала двигаемся по столбцу
                for j in range(N): # потом по строкам, чтоб найти сумму по оси y
                    localsum += li[j][i] #читаем таблицу сверху вниз слева направо
                if localsum >= maxsum: #ну это у нас обычный счетчик
                    maxsum = localsum
                    curIndex = i # ловим айдишник солбца максимальной суммы на данный момент
                localsum = 0
            print("Столбец с наибольшей суммой элементов: ", curIndex + 1) # и тут юзаем его
            return 0
    except:
        print("Неверно введены значения.")
        print("")
        f()
f()