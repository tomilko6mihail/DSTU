#include <iostream>
#include <string>
#include <ctime>
#include <random>

using namespace std;
bool validation(const string str){
    bool flagDot = false;
    if (str.empty()){
        cout << "Строка не может быть пустой" << endl;
        return false;
    }
    int startIndex = str[0] == '-' ? 1 : 0;
    
    
    for (int i = startIndex; i < str.length(); i++ ){
        if(str[0] == '.'){
            cout << "Число не должно начинаться с точки" << endl;
            return false;
        }
        if(isspace(str[i])){
            cout << "Число не должно содержать пробелы" << endl;
            return false;
        }
        if (str[i] == '.'){
            if(flagDot){
                cout << "В числе может быть лишь одна точка" << endl;
                return false;
            }
            flagDot = true;
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

int main() {
    srand(time(0));
// Задание 1
//    int k = rand() % (9 - 1 + 1) + 1;
//    int l = rand() % (9 - k + 1) + k;
//    int ma[10];
//    int *index;
//    index = ma;
//    for(int i = 0; i < 10; i++){
//        *(index + i) = rand() % (10 - 1 + 1) + 1;
//    }
//    cout << "Текущий вид массива: " << endl;
//    cout << "[";
//    for(int i = 0; i < 10; i++){
//        cout << ma[i] << " ";
//    }
//    cout << "]" << endl;
//    
//    index = ma;
//    float sumValues = 0;
//    
//    for(int i = k; i <= l; i++){
//        index = ma + i;
//        sumValues += *index;
//    }
//    cout << "Среднее арифметическое от индекса " << k << " до индекса " << l << ": " << sumValues / (l - k + 1) << endl;
    
////Задание 2
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(1.0, 10.0);
    
met1:
    string userNumberString = "";
    float userNumber = 0.0;
    cout << "Введите чсило С: ";
    cin >> userNumberString;
    if(validation(userNumberString)){
        userNumber = stof(userNumberString);
    } else{
        goto met1;
    }
    float ma[10];
    float *index;
    index = ma;
    for(int i = 0; i < 10; i++){
        *(index + i) = dis(gen);
    }
    cout << "Текущий вид массива: " << endl;
    cout << "[";
    for(int i = 0; i < 10; i++){
        cout << ma[i] << " ";
    }
    cout << "]" << endl;
    
    index = ma;

    int count = 0;
    
    for(int i = 0; i < 10; i++){
        index = ma + i;
        if(*index < userNumber){
            count += 1;
        }
    }
    cout << "Таких чисел " << count << endl;
}
