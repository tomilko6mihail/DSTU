#include <iostream>
#include <vector>
#include <cstring>
#include <random>
#include <algorithm>

using namespace std;

const int TOTAL_SIZE = 1171;
const int MAP_SIZE = 147;
unsigned char memory[TOTAL_SIZE];

std::random_device rd;
std::mt19937 gen(rd());

void set_bit(int bit, bool val) {
    int byte_idx = bit / 8;
    int bit_pos = bit % 8;
    if (val) {
        memory[byte_idx] |= (1u << bit_pos);
    } else {
        memory[byte_idx] &= ~(1u << bit_pos);
    }
}

bool get_bit(int bit) {
    int byte_idx = bit / 8;
    int bit_pos = bit % 8;
    return (memory[byte_idx] & (1u << bit_pos)) != 0;
}

void init_memory() {
    memset(memory, 0, TOTAL_SIZE);
    for (int i = 0; i < MAP_SIZE; ++i) {
        set_bit(i, true);
    }
}

void allocate(size_t size) {
    if (size == 0) {
        cout << "Недостаточно памяти" << endl;
        return;
    }
    
    vector<int> suitable_positions;
    
    for (int i = 0; i <= TOTAL_SIZE - static_cast<int>(size); ++i) {
        bool fits = true;
        for (size_t j = 0; j < size; ++j) {
            if (get_bit(i + j)) {
                fits = false;
                break;
            }
        }
        if (fits) {
            suitable_positions.push_back(i);
        }
    }
    
    if (!suitable_positions.empty()) {
        uniform_int_distribution<> dis(0, suitable_positions.size() - 1);
        int selected_pos = suitable_positions[dis(gen)];
        
        for (size_t j = 0; j < size; ++j) {
            set_bit(selected_pos + j, true);
        }
        cout << selected_pos << endl;
    } else {
        cout << "Недостаточно памяти" << endl;
    }
}

void release(int addr) {
    if (addr < 0 || addr >= TOTAL_SIZE) {
        return;
    }
    int block_size = 0;
    while (addr + block_size < TOTAL_SIZE && get_bit(addr + block_size)) {
        ++block_size;
    }
    for (int j = 0; j < block_size; ++j) {
        set_bit(addr + j, false);
    }
}

void get_info() {
    vector<pair<int, int>> free_segments;
    vector<pair<int, int>> occupied_segments;
    int total_free = 0;
    int total_occupied = 0;
    int i = 0;
    while (i < TOTAL_SIZE) {
        bool is_occupied = get_bit(i);
        int start = i;
        int seg_size = 0;
        while (i < TOTAL_SIZE && get_bit(i) == is_occupied) {
            ++seg_size;
            ++i;
        }
        if (is_occupied) {
            occupied_segments.push_back({start, seg_size});
            total_occupied += seg_size;
        } else {
            free_segments.push_back({start, seg_size});
            total_free += seg_size;
        }
    }
    cout << "Количество занятых участков: " << occupied_segments.size() << endl;
    for (const auto& seg : occupied_segments) {
        cout << "Начальный адрес: " << seg.first << ", Размер: " << seg.second << endl;
    }
    cout << "Количество свободных участков: " << free_segments.size() << endl;
    for (const auto& seg : free_segments) {
        cout << "Начальный адрес: " << seg.first << ", Размер: " << seg.second << endl;
    }
    cout << "Общее количество занятой памяти: " << total_occupied << endl;
    cout << "Общее количество свободной памяти: " << total_free << endl;
}

int main() {
    init_memory();
    get_info();
    allocate(100);
    allocate(200);
    get_info();
    release(147);
    get_info();
    allocate(150);
    get_info();
    return 0;
}
