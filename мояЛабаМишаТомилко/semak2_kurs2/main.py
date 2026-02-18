#1 задание
while True:
    text = input("Введите текст, оканчивающийся точкой: ")
    letters = set()
    result = set()
    letterss = 'abcdefghijklmnopqrstuvwxyz.'
    isValidText = False
    for letter in text:
        if letter not in letterss:
            isValidText = False
            break
        else:
            isValidText = True
    if isValidText:
        if '.' in text:
            for char in text:
                if char == '.':
                    break
                if char == ' ':
                    pass
                else:
                    if char in letters:
                        result.add(char)
                    else:
                        letters.add(char)
            break
        else:
            print("Текст должен оканчиваться на точку!!")
    else:
        print("Должны быть только латинские строчные буквы")
if not result:
    print("В тексте нет повторяющихся букв")
else:
    print("Символы, найденые 2 или более раз: ", ', '.join(sorted(result)))

#2 задание
while True:
    text2 = input("Введите непустую последовательность символов: ")
    if '' == text2:
        print("Последовательность не должна быть пустой!!")
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
if not result2:
    print("В тексте нет букв, удовлетворяющих условию")
else:  
    print("Найденые нужные символы: ", ', '.join(sorted(result2)))
    print(len(result2), " - длина множества")
