try:
    def Mean(X, Y):
        AMean = (X+Y)/2
        GMean = (X*Y)**(1/2)
        print("AMean: ", AMean, "GMean: ", GMean)
    A = float(input("Введите A: "))
    B = float(input("Введите B: "))
    C = float(input("Введите C: "))
    D = float(input("Введите D: "))
    li = [B, C, D]
    for i in range(len(li)):
        Mean(A, li[i])
except:
    print("Неверно введены значения")

    