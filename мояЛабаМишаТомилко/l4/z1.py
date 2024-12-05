a = float(input("Введите отрезок а: "))
b = float(input("Введите отрезок b: "))
c = float(input("Введите отрезок c: "))
d = float(input("Введите отрезок d: "))

def Triangle(x, y, z):
    for i in range(3):
        li = [x, y, z]
        number = li[i]
        li.remove(li[i])
        print(li)
        if number >= li[0] + li[1]:
            return 0
    print("S = ", (x*y)/z)
print(Triangle(3, 8, 6))