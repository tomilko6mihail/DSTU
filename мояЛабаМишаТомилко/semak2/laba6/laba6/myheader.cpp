#include "myheader.h"
#include <iostream>
#include <limits>
#include <locale>
#include <codecvt>

using namespace std;

bool students::validationInt(const string str) {
    if (str.empty()){
        cout << "Строка не может быть пустой" << endl;
        return false;
    }
    int startIndex = str[0] == '-' ? 1 : 0;
    
    
    for (int i = startIndex; i < str.length(); i++ ){
        if(str[0] == '.'){
            cout << "Число не должно содержать точку" << endl;
            return false;
        }
        if(isspace(str[i])){
            cout << "Число не должно содержать пробелы" << endl;
            return false;
        }
        if (str[i] == '.'){
            cout << "Число не должно содержать точку" << endl;
            return false;
        }
        else if (!isdigit(str[i])){
            cout << "В числе не могут быть буквы" << endl;
            return false;
        }
    }
    if(startIndex < str.length()){
        return true;
    }
    else{
        cout << "Минус это не число" << endl;
        return false;
    }
}

void students::addStudents(){
    cout << "----------Добавление студентов---------" << endl;
    
    count_students:
    string strN;
    int N;
    
    while (true) {
        cout << "Введите количество студентов: ";
        string strN;
        getline(cin, strN);
        
        if (validationInt(strN)) {
            N = stoi(strN);
            break;
        }
    }
    
    for(int i = 0; i < N; i++){
        cout << "Студент " << i + 1 << ": " << endl;
        
        string c_fio, subject, str_peresd;
        int c_peresd;
        
        
        cout << "Введите ФИО через пробел: ";
        getline(cin, c_fio);
        
        cout << "Введите предмет: ";
        getline(cin, subject);
    
        while (true) {
            cout << "Введите количество пересдач: ";
            string str_peresd;
            getline(cin, str_peresd);
            
            if (validationInt(str_peresd)) {
                c_peresd = stoi(str_peresd);
                break;
            }
        }
        
        studenti.emplace_back(c_fio, subject, c_peresd);
        
    }
    
}

bool students::sortirovka(const student& a, const student& b) {
    return a.count_peresdachi > b.count_peresdachi; // Сортировка по убыванию
}

void students::displayStudents(){
    sort(studenti.begin(), studenti.end(), sortirovka);
    for(const auto& stud : studenti){
        cout << "ФИО студента: " << stud.fio << endl;
        cout << "Предмет: " << stud.subj << endl;
        cout << "Количество пересдач: " << stud.count_peresdachi << endl;
        cout << "-----------------------------------------" << endl;
    }
}


void students::mapBukvi(){
    ifstream file("text.txt");
    
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
    } else {
        string text;
        string alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        while (getline(file, text)) {
            for (char c : text) {
                if (alph.find(c) != string::npos){
                    mapa[c] += 1;
                }
            }
        }
    }
    
    for (const auto& [ch, numb] : mapa) {
        cout << ch << ": " << numb << endl;
    }

}
