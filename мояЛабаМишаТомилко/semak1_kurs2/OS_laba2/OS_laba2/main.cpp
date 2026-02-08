#include <iostream>
using namespace std;

unsigned char RAM[128];
unsigned char DISK[128];
const int PAGE_SIZE = 16;
const int NUM_PAGES = 4;

struct PageInfo {
    int disk_page;
    int counter;
    bool second_chance;
};

PageInfo getPageInfo(int ram_page) {
    PageInfo info;
    int base = ram_page * PAGE_SIZE;
    info.disk_page = RAM[base];
    info.counter = RAM[base + 1];
    return info;
}

void setPageInfo(int ram_page, const PageInfo& info) {
    int base = ram_page * PAGE_SIZE;
    RAM[base] = info.disk_page;
    RAM[base + 1] = info.counter;
}

int findPageToEvict() {
    int min_counter = 256;
    int page_to_evict = 0;
    
    for (int i = 0; i < NUM_PAGES; i++) {
        PageInfo info = getPageInfo(i);
        if (info.disk_page != -1) {
            if (info.second_chance) {
                info.second_chance = 0;
                setPageInfo(i, info);
                continue;
            }
            if (info.counter < min_counter) {
                min_counter = info.counter;
                page_to_evict = i;
            }
        }
    }
    
    return page_to_evict;
}

int findPageInRAM(int virtual_page) {
    for (int i = 0; i < NUM_PAGES; i++) {
        if (getPageInfo(i).disk_page == virtual_page) {
            return i;
        }
    }
    return -1;
}

void giveSecondChance(int page_num) {
    int ram_page = findPageInRAM(page_num);
    if (ram_page != -1) {
        PageInfo info = getPageInfo(ram_page);
        info.second_chance = 1;
        setPageInfo(ram_page, info);
    }
}


void loadPage(int virtual_page, int ram_page) {
    for (int i = 0; i < PAGE_SIZE; i++) {
        RAM[ram_page * PAGE_SIZE + i] = DISK[virtual_page * PAGE_SIZE + i];
    }
    
    PageInfo new_info;
    new_info.disk_page = virtual_page;
    new_info.counter = 128;
    setPageInfo(ram_page, new_info);
}

void accessPage(int page_num) {
    for (int i = 0; i < NUM_PAGES; i++) {
        PageInfo info = getPageInfo(i);
        if (info.disk_page == page_num) {
            info.counter |= 128;
            setPageInfo(i, info);
            cout << "Страница " << page_num << " в RAM, счетчик: " << info.counter << endl;
            return;
        }
    }
    
    int ram_page = findPageToEvict();
    cout << "Выгружаем страницу " << getPageInfo(ram_page).disk_page << endl;
    loadPage(page_num, ram_page);
    cout << "Загружаем страницу " << page_num << " в RAM[" << ram_page << "]" << endl;
}

void updateCounters() {
    for (int i = 0; i < NUM_PAGES; i++) {
        PageInfo info = getPageInfo(i);
        if (info.disk_page != -1) {
            info.counter >>= 1;
            setPageInfo(i, info);
        }
    }
}

void displayPages() {
    cout << "RAM\tVirt\tCounter" << endl;
    for (int i = 0; i < NUM_PAGES; i++) {
        PageInfo info = getPageInfo(i);
        if (info.disk_page != -1) {
            cout << i << "\t" << info.disk_page << "\t" << info.counter << endl;
        }
    }
}

int main() {
    for (int i = 0; i < NUM_PAGES; i++) {
        PageInfo info;
        info.disk_page = -1;
        info.counter = 0;
        setPageInfo(i, info);
    }
    
    accessPage(1);
    accessPage(2);
    accessPage(3);
    accessPage(4);
    
    updateCounters();
    accessPage(1);
    
    updateCounters();
    accessPage(5);
    
    displayPages();
    
    return 0;
}
//дать шанс второй странице - если ее попытаются выгрузить, повышаем бит на единичку чтоб не выгрузили, а если снов апопалась, то все
