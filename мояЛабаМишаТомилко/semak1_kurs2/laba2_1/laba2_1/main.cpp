#include "finance_tracker.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    FinanceTracker tracker;
    
    string cStr;
    int c;
    
    while (true) {
        cout << "----------Менеджер личных финансов----------" << endl;
        cout << "1. Добавить транзакцию" << endl;
        cout << "2. Сгенерировать отчет за день" << endl;
        cout << "3. Проверить ID транзакции" << endl;
        cout << "4. Генерировать тестовые данные" << endl;
        cout << "0. Выход" << endl;
        cout << "Выбор: ";
        getline(cin, cStr);
        if (tracker.validationInt(cStr)) {
            c = stoi(cStr);
        } else {
            continue;
        }
        
        switch (c) {
            case 1:
                tracker.addTransaction();
                break;
            case 2:
                tracker.generateDailyReport();
                break;
            case 3:
                tracker.checkTransactionID();
                break;
            case 4:
                tracker.generateRandomData();
                break;
            case 0:
                return 0;
            default:
                cout << "Нет такого варианта." << endl;
        }
    }
    return 0;
}
