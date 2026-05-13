import random
import time

def insertion_sort(arr):
    a = arr[:]
    iterations, comparisons, swaps = 0, 0, 0
    for i in range(1, len(a)):
        iterations += 1
        key = a[i]
        j = i - 1
        while j >= 0:
            comparisons += 1
            if a[j] > key:
                a[j + 1] = a[j]
                swaps += 1
                j -= 1
            else:
                break
        a[j + 1] = key
    return iterations, comparisons, swaps

def selection_sort(arr):
    a = arr[:]
    iterations, comparisons, swaps = 0, 0, 0
    for i in range(len(a)):
        iterations += 1
        min_idx = i
        for j in range(i + 1, len(a)):
            comparisons += 1
            if a[j] < a[min_idx]:
                min_idx = j
        if min_idx != i:
            a[i], a[min_idx] = a[min_idx], a[i]
            swaps += 1
    return iterations, comparisons, swaps

def bubble_sort(arr):
    a = arr[:]
    iterations, comparisons, swaps = 0, 0, 0
    for i in range(len(a)):
        iterations += 1
        for j in range(len(a) - i - 1):
            comparisons += 1
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
                swaps += 1
    return iterations, comparisons, swaps

def quick_sort(arr):
    a = arr[:]
    iterations, comparisons, swaps = 0, 0, 0

    def qsort(a, low, high):
        nonlocal iterations, comparisons, swaps
        if low < high:
            iterations += 1
            pivot = a[high]
            i = low - 1
            for j in range(low, high):
                comparisons += 1
                if a[j] <= pivot:
                    i += 1
                    a[i], a[j] = a[j], a[i]
                    swaps += 1
            a[i + 1], a[high] = a[high], a[i + 1]
            swaps += 1
            qsort(a, low, i)
            qsort(a, i + 2, high)

    import sys
    sys.setrecursionlimit(20000)
    qsort(a, 0, len(a) - 1)
    return iterations, comparisons, swaps

def run(func, arr):
    start = time.perf_counter()
    it, comp, sw = func(arr)
    t = time.perf_counter() - start
    return it, comp, sw, t

def v26(arr):
    a = arr[:]
    positions = [i for i in range(len(a)) if a[i] % 4 == 0]
    values = [a[i] for i in positions]
    for i in range(len(values)):
        for j in range(len(values) - i - 1):
            if values[j] > values[j + 1]:
                values[j], values[j + 1] = values[j + 1], values[j]
    for i in range(len(positions)):
        a[positions[i]] = values[i]
    return a

def study_size():
    print("\n--- Исследование 1: зависимость от размера ---\n")
    sizes = [20, 500, 1000, 3000, 5000, 10000]
    methods = [("прямое включение", insertion_sort), ("прямой выбор", selection_sort), ("прямой обмен", bubble_sort), ("метод Хоара", quick_sort)]
    for name, func in methods:
        print(f"Метод: {name}")
        print(f"{'Размер':>8} {'Итерации':>12} {'Сравнения':>12} {'Перест.':>10} {'Время':>12}")
        for n in sizes:
            arr = [random.randint(1, 10000) for _ in range(n)]
            it, comp, sw, t = run(func, arr)
            print(f"{n:>8} {it:>12} {comp:>12} {sw:>10} {t:>12.6f}")
        print()

def study_order():
    print("\n--- Исследование 2: влияние начальной упорядоченности ---\n")
    while True:
        try:
            n = int(input("Введите размер массива N: "))
            if n > 0:
                break
        except ValueError:
            pass
        print("Ошибка, введите целое число больше 0")

    base = list(range(1, n + 1))
    random_arr = [random.randint(1, 10000) for _ in range(n)]
    sorted_arr = base[:]
    reversed_arr = list(reversed(base))

    def gen_arr(arr, precent):
        res = arr[:]
        start_idx = int(len(arr) * precent)
        rand = res[start_idx:]
        random.shuffle(rand)
        res[start_idx:] = rand
        return res

    p25 = gen_arr(base,  0.25)
    p50 = gen_arr(base,  0.5)
    p75 = gen_arr(base,  0.75)

    states = [("Random", random_arr), ("Sorted", sorted_arr), ("Reversed", reversed_arr),
              ("25%", p25), ("50%", p50), ("75%", p75)]
    methods = [("Включение", insertion_sort), ("Выбор", selection_sort), ("Обмен", bubble_sort), ("Хоар", quick_sort)]

    print(f"{'Состояние':<12}", end="")
    for name, _ in methods:
        print(f"{name:>22}", end="")
    print()
    print(f"{'':12}", end="")
    for _ in methods:
        print(f"{'перест. / время':>22}", end="")
    print()
    print("-" * 100)

    for state_name, arr in states:
        print(f"{state_name:<12}", end="")
        for _, func in methods:
            it, comp, sw, t = run(func, arr)
            cell = f"{sw} / {t:.4f}"
            print(f"{cell:>22}", end="")
        print()

def run_v26():
    print("\n--- Мой вариант: сортировка кратных 4 (прямой обмен) ---\n")
    choice = input("1 - ввести массив вручную, 2 - сгенерировать: ").strip()
    if choice == "1":
        while True:
            try:
                arr = list(map(int, input("Введите числа через пробел: ").split()))
                if arr:
                    break
            except ValueError:
                pass
            print("Ошибка")
    else:
        while True:
            try:
                n = int(input("Размер массива: "))
                if n > 0:
                    break
            except ValueError:
                pass
            print("Ошибка")
        arr = [random.randint(-20, 50) for _ in range(n)]

    print(f"\nИсходный:  {arr}")
    result = v26(arr)
    print(f"Результат: {result}")
    print(f"\nКратные 4 до:    {[x for x in arr if x % 4 == 0]}")
    print(f"Кратные 4 после: {[x for x in result if x % 4 == 0]}")

print("1 - Исследование по размеру")
print("2 - Исследование по упорядоченности")
print("3 - Мой вариант")
print("4 - Всё сразу")

choice = input("Выбор: ").strip()
if choice == "1":
    study_size()
elif choice == "2":
    study_order()
elif choice == "3":
    run_v26()
elif choice == "4":
    study_size()
    study_order()
    run_v26()