#1 задание
while True:
    text = input("Введите текст, оканчивающийся точкой: ")
    letters = set()
    result = set()
    
    if '.' in text:
        for char in text:
            if char == '.':
                break
            if char in letters:
                result.add(char)
            else:
                letters.add(char)
        break
    else:
        print("Текст должен оканчиваться на точку!!")
if not result:
    print("В тексте нет повторяющихся букв")
else:
    print("Символы, найденые 2 или более раз: ", ', '.join(sorted(result)))

#2 задание
while True:
    text2 = input("Введите непустую последовательность символов: ")
    if ' ' in text2:
        print("Последовательность не должна включать пробел!!")
    else:
        result2 = set()
        need_char_set = set()

        for i in range(ord('T'), ord('X') + 1):
            need_char_set.add(chr(i))

        for i in range(ord('1'), ord('4') + 1):
            need_char_set.add(chr(i))

        for char in text2:
            if char in need_char_set:
                result2.add(char)
        break

print("Найденые нужные символы: ", ', '.join(sorted(result2)))
