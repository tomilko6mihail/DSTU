#1


# print(f'Количество букв А: {len([i for i in input('Введите ваш текст: ') if i == 'А'])}')

#2

# ascii = 'АБВГДЕËЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмеопрстуфхцчшщъыьэюя0123456789'
# words, total = input('Введите ваш текст: '), 0
# for elem in range(len(words)):
#     if words[elem - 1] in ascii and words[elem] == ' ':
#         total += 1
# if words[len(words) - 1] == ' ':
#     total -= 1
# print(f'Количесвто слов в тексте: {total + 1}')

#3
# try:
#     digits, count = [float(input()) for _ in range(int(input('Введите количество чисел (вводите через Enter): ')))], 0
#     a, b = float(input('Введите левую грницу: ')), float(input('Введите правую границу: '))

#     for el in digits:
#         if el >= 0:
#             count += el
#         else:
#             break
#     print(f'Сумма элементов до первого элемента: {count}')

#     result, d = [], 0
#     for ty in range(len(digits)):
#         if digits[ty] < 0:
#             digits[ty] *= -1
#         if a <= digits[ty] <= b:
#             result = digits[ty + 1:]
#             d += 1

#     result += '0' * d
#     result = [int(i) for i in result]

#     print(result)
# except:
#     print('ВВЕДЕНО НЕВЕРНОЕ ЗНАЧЕНИЙЕ')
