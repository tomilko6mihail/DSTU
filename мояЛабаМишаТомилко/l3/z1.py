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
            for i in range(M):
                for j in range(N):
                    localsum += li[j][i]
                if localsum >= maxsum:
                    maxsum = localsum
                    curIndex = i
                localsum = 0
            print(curIndex + 1, 90)
            return 0
    except:
        print("Неверно введены значения.")
        print("")
        f()
f()