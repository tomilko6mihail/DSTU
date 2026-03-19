import os

def zad1(input_file, output_file):
    if not os.path.exists(input_file):
        print(f"Ошибка: Файл '{input_file}' не найден!")
        return
    
    with open(input_file, "r", encoding="utf-8") as f:
        lines = f.readlines()

    if not lines:
        print("Файл пуст")
        return
    
    result = []
    for line in lines:
        words = line.strip().split()
        to_remove = [i for i, w in enumerate(words) if 3 <= len(w) <= 5]
        
        if len(to_remove) % 2:
            to_remove = to_remove[:-1]
        
        new_line = ' '.join([w for i, w in enumerate(words) if i not in to_remove])
        result.append(new_line)

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('\n'.join(result))
    
    with open(output_file, 'r', encoding='utf-8') as f:
        processed_content = f.read()
    
    with open(input_file, 'w', encoding='utf-8') as f:
        f.write(processed_content)
    
    os.remove(output_file)
    
    print(f"Файл {input_file} успешно обновлен")

def zad2(input_file, output_file='G.txt'):

    if not os.path.exists(input_file):
        print(f"Ошибка: Файл '{input_file}' не найден")
        return
    
    with open(input_file, 'r', encoding='utf-8') as f:
        testLines = f.readlines()

    if not testLines:
        print("Файл пуст")
        return
    
    while True:
        s = input("Введите подстроку: ").strip()
        if not s:
            continue
            
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        filtered = [line for line in lines if s in line]
        
        if filtered:
            with open(output_file, "w", encoding="utf-8") as f:
                f.writelines(filtered)
        else:
            print("Подстрока не найдена")
            if input("Повторить? (да/нет): ").lower() != 'да':
                break

zad1('failik.txt', 'failik_new.txt')
zad2('F.txt')