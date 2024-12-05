from itertools import *
try:
    a = float(input("Введите отрезок а: "))
    b = float(input("Введите отрезок b: "))
    c = float(input("Введите отрезок c: "))
    d = float(input("Введите отрезок d: "))
    li = [a, b, c, d]
    def Triangle(x, y, z):
        for i in range(3):
            li = [x, y, z]
            number = li[i]
            li.remove(li[i])
            if number >= li[0] + li[1]:
                return -1
        # по формуле Герона (впервые вижу такую)
        poluPerimetr = (x + y + z) / 2
        return (poluPerimetr * (poluPerimetr - x) * (poluPerimetr - y) * (poluPerimetr - z))**0.5

    for i in permutations(li, 3):
        print("Площадь треугольника для аргументов x =", i[0], ", y =", i[1], ", z =", i[2], ": ", Triangle(i[0], i[1], i[2]))
except:
    print("Неверно введены значения.")
