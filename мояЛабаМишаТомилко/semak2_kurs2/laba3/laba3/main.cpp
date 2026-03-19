
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>

using namespace std;

struct MemorySegment {
    void* memory;
    bool* used;
    size_t size;
    size_t used_count;
    
    MemorySegment(size_t s) : size(s), used_count(0) {
        memory = malloc(size);
        used = new bool[size]();
    }
    
    ~MemorySegment() {
        free(memory);
        delete[] used;
    }
};

MemorySegment segment1(1024);
MemorySegment segment2(1024);

struct Pointer {
    void* addr;
    size_t size;
    MemorySegment* segment;
    bool is_valid;
    
    Pointer() : addr(nullptr), size(0), segment(nullptr), is_valid(false) {}
};

Pointer pointers[100];
int pointer_count = 0;

template<typename T>
T getValidInput(const string& prompt, T min_val, T max_val) {
    T value;
    bool valid = false;
    
    do {
        cout << prompt;
        cin >> value;
        
        if (cin.fail() || value < min_val || value > max_val) {
            cout << "Ошибка ввода! Введите число от " << min_val << " до " << max_val << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            valid = true;
        }
    } while (!valid);
    
    return value;
}

Pointer* NewPointer(void* p, size_t bytes) {
    if (pointer_count >= 100) {
        cout << "Ошибка: достигнут максимум указателей" << endl;
        return nullptr;
    }

    MemorySegment* selected_segment = nullptr;
    
    if (segment1.used_count + bytes <= segment1.size) {
        selected_segment = &segment1;
    } else if (segment2.used_count + bytes <= segment2.size) {
        selected_segment = &segment2;
    } else {
        cout << "Ошибка: недостаточно памяти в сегментах" << endl;
        return nullptr;
    }
    
    size_t offset = 0;
    for (size_t i = 0; i < selected_segment->size - bytes; i++) {
        bool free_space = true;
        for (size_t j = 0; j < bytes; j++) {
            if (selected_segment->used[i + j]) {
                free_space = false;
                break;
            }
        }
        if (free_space) {
            offset = i;
            break;
        }
    }
    
    for (int i = 0; i < pointer_count; i++) {
        if (pointers[i].addr == p) {
            pointers[i].is_valid = false; //выключаем старый указатель, якобы теряя доступ к занятой памяти. после этого создаем совершенно новый указатель с новым адресом и ссылкой на новые данные
            cout << "Утечка памяти, старая память не освобождена" << endl;
            break;
        }
    }
    
    Pointer* new_ptr = &pointers[pointer_count++];
    new_ptr->addr = static_cast<char*>(selected_segment->memory) + offset;
    new_ptr->size = bytes;
    new_ptr->segment = selected_segment;
    new_ptr->is_valid = true;
    
    for (size_t i = 0; i < bytes; i++) {
        selected_segment->used[offset + i] = true;
    }
    selected_segment->used_count += bytes;
    
    cout << "Память выделена в сегменте " << (selected_segment == &segment1 ? "1" : "2") << " по адресу " << new_ptr->addr << endl;
    
    return new_ptr;
}

void WritePointer(Pointer* p, void* value, size_t bytes) {
    if (!p || !p->is_valid || !p->addr) {
        cout << "Ошибка: некорректный указатель" << endl;
        return;
    }
    
    if (bytes > p->size) {
        cout << "Ошибка: размер значения превышает выделенную память" << endl;
        return;
    }
    
    memcpy(p->addr, value, bytes);
    cout << "Новое значение записано по адресу " << p->addr << endl;
}

void* ReadPointer(Pointer* p) {
    if (!p || !p->is_valid || !p->addr) {
        cout << "Ошибка: некорректный указатель" << endl;
        return nullptr;
    }
    
    return p->addr;
}

void SetPointer(Pointer* to, Pointer* from) {
    if (!to || !from || !to->is_valid || !from->is_valid) {
        cout << "Ошибка: некорректные указатели" << endl;
        return;
    }
    
    if (from->size > to->size) {
        cout << "Ошибка: недостаточно памяти в целевом указателе" << endl;
        return;
    }
    
    memcpy(to->addr, from->addr, from->size);
    cout << "Значение скопировано из " << from->addr << " в " << to->addr << endl;
}

void FreePointer(Pointer* p) {
    if (!p || !p->is_valid || !p->addr) {
        cout << "Ошибка: некорректный указатель" << endl;
        return;
    }
    
    char* addr = static_cast<char*>(p->addr);
    char* seg_start = static_cast<char*>(p->segment->memory);
    size_t offset = addr - seg_start; // offset - смещение назад
    
    for (size_t i = 0; i < p->size; i++) {
        p->segment->used[offset + i] = false;
    }
    p->segment->used_count -= p->size;
    
    p->is_valid = false;
    p->addr = nullptr;
    
    cout << "Память освобождена" << endl;
}

void task1() {
    cout << "\n=== 1 задание ===\n";
    
    int N = getValidInput<int>("Введите размер массива N: ", 1, 100);
    
    Pointer* arr_ptr = NewPointer(nullptr, N * sizeof(int));
    if (!arr_ptr) return;
    
    int* arr = static_cast<int*>(arr_ptr->addr);
    cout << "Сгенерированный массив: ";
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 21 - 10;
        cout << arr[i] << " ";
    }
    cout << endl;
    
    int first_pos = -1, second_pos = -1;
    for (int i = 0; i < N; i++) {
        if (arr[i] > 0) {
            if (first_pos == -1) {
                first_pos = i;
            } else if (second_pos == -1) {
                second_pos = i;
                break;
            }
        }
    }
    
    if (first_pos == -1 || second_pos == -1) {
        cout << "В массиве меньше двух положительных элементов" << endl;
    } else {
        int sum = 0;
        for (int i = first_pos + 1; i < second_pos; i++) {
            sum += arr[i];
        }

        cout << "Сумма элементов между первыми двумя положительными элементами: " << sum << endl;
    }
    
    FreePointer(arr_ptr);
}

void task2() {
    cout << "\n=== 2 задание ===\n";
    
    int N = getValidInput<int>("Введите размер массива N: ", 1, 100);
    
    Pointer* arr_ptr = NewPointer(nullptr, N * sizeof(double));
    if (!arr_ptr) return;
    
    double* arr = static_cast<double*>(arr_ptr->addr);
    cout << "Сгенерированный массив: ";
    for (int i = 0; i < N; i++) {
        arr[i] = (rand() % 2001 - 1000) / 100.0;
        cout << arr[i] << " ";
    }
    cout << endl;
    
    double max_value = 0;
    
    for (int i = 0; i < N; i++) {
        if (abs(arr[i]) > abs(max_value)) {
            max_value = arr[i];
        }
    }
    
    cout << "Максимальный по модулю элемент:  " << max_value << endl;
    
    FreePointer(arr_ptr);
}

void demonstrateLeak() {
    cout << "\n=== работа с указателями ===\n";
    
    Pointer* p1 = NewPointer(nullptr, sizeof(int));
    Pointer* p2 = NewPointer(nullptr, sizeof(int));
    
    if (!p1 || !p2) return;
    
    int val1 = 42;
    int val2 = 100;
    
    WritePointer(p1, &val1, sizeof(int));
    WritePointer(p2, &val2, sizeof(int));
    
    int* read1 = static_cast<int*>(ReadPointer(p1));
    int* read2 = static_cast<int*>(ReadPointer(p2));
    
    if (read1 && read2) {
        cout << "Значение p1: " << *read1 << endl;
        cout << "Значение p2: " << *read2 << endl;
    }
    
    SetPointer(p1, p2);

    read1 = static_cast<int*>(ReadPointer(p1));
    if (read1) {
        cout << "После копирования p1 = p2: p1 = " << *read1 << endl;
    }

    cout << "\nСоздаем указатель p3 со значением 555:" << endl;
    Pointer* p3 = NewPointer(nullptr, sizeof(int));
    int val3 = 555;
    WritePointer(p3, &val3, sizeof(int));
    
    cout << "Создаем новый указатель p4 с адресом p3(утечка):" << endl;
    Pointer* p4 = NewPointer(p3->addr, sizeof(int));
    
    FreePointer(p1);
    FreePointer(p2);
    FreePointer(p3);
}

int main() {
    srand(time(nullptr));

    int k;
    do {
        cout << "1. Задание 1\n";
        cout << "2. Задание 2\n";
        cout << "3. Демонстрация работы с указателями\n";
        cout << "0. Выход\n";
        
        k = getValidInput<int>("Выберите пункт: ", 0, 3);
        
        switch (k) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                demonstrateLeak();
                break;
            case 0:
                cout << "Программа завершена \n";
                break;
        }
    } while (k != 0);
    
    return 0;
}
