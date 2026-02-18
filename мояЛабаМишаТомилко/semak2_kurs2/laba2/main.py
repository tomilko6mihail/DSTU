def zad1(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
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

def zad2(input_file, output_file='G.txt'):
    while True:
        s = input("Введите подстроку: ").strip()
        if not s:
            continue
            
        with open(input_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        filtered = [line for line in lines if s in line]
        
        if filtered:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.writelines(filtered)
            print(f"Файл {output_file} успешно создан")
            break
        else:
            print("Подстрока не найдена")
            if input("Повторить? (да/нет): ").lower() != 'да':
                break

zad1('failik.txt', 'failik_new.txt')
zad2('F.txt')