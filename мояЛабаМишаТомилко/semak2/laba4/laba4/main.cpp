#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main(){
    setlocale(0, "");
    string stroka;
    cout << "Введите строкy для 1 задания: ";
    getline(cin, stroka);
    
    int c = 0;
    
    for(int i = 0; i < stroka.length(); i++){
        if(isdigit(stroka[i])){
            c++; // пасхалко
        }
    }
    cout << "Количество цифр в строке: " << c << endl;
    
    string stroka2;
    cout << "Введите строкy для 2 задания: ";
    getline(cin, stroka2);
    
    string stroka3 = "";
    
    for(int i = 0; i < stroka2.length(); i++){
        char letter[1];
        letter[0] = tolower(stroka2[i]);
        string letter2;
        letter2 = string(letter);
        stroka3.append(letter2);
    }
    
    cout << "Строка с нижним регистром: " << stroka3 << endl;
}
