from random import randint
try:
    c = int(input("Введите длину списка: "))
    li = [randint(-20, 20) for i in range(c)]
    print(li)
    print(max(max(li[:int(len(li)/2)]), max(li[int(len(li)/2):])))
except:
    print("Неверно введены значения")