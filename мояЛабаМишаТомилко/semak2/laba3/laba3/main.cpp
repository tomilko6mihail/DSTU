#include <iostream>
#include <random>
#include <string>
#include <cmath>

using namespace std;

bool validation(const string str){
    if (str.empty()){
        cout << "Строка не может быть пустой" << endl;
        return false;
    }
    int startIndex = str[0] == '-' ? 1 : 0;
    
    for (int i = startIndex; i < str.length(); i++ ){
        if(isspace(str[i])){
            cout << "Число не должно содержать пробелы" << endl;
            return false;
        }
        if (str[i] == '.'){
            cout << "В числе не может быть точки" << endl;
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

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10.0, 10.0);
    
    met1:
    string lenStr;
    int len;
    cout << "Введите размер массива: ";
    cin >> lenStr;
    
    if(validation(lenStr)){
        len = stoi(lenStr);
    }else{
        goto met1;
    }
    
    float *arr = new float[len];
    
    for(int i = 0; i < len; i++){
        arr[i] = dis(gen);
    }
    
    for(int i = 0; i < len; i++){
        cout << arr[i] << " ";
    }
    
    float min = 10.0;
    int currentIdmin = 0;
    float max = 0.0;
    int currentIdmax = 0;
    
    for(int i = 0; i < len; i++){
        if(fabs(arr[i]) < min){
            min = fabs(arr[i]);
            currentIdmin = i;
        }
        if(fabs(arr[i]) > max){
            max = fabs(arr[i]);
            currentIdmax = i;
        }
    }
    
    float proizved = 1.0;
    
    for(int i = currentIdmax + 1; i < len; i++){
        proizved *= arr[i];
    }
    
    float *arr2 = new float[len];
    
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            if(arr[j] < 0){
                arr2[i] = arr[j];
            }
        }
    }
    
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    cout << endl;
    cout << "1)"<<endl;
    cout << "Номер минимального по модулю числа: " << currentIdmin << endl;
    cout << "Номер максимального по модулю числа: " << currentIdmax << endl;
    cout << "2)"<<endl;
    if(currentIdmax == len - 1){
        cout << "После максимального элемента ничего нет" << endl;
    } else {
        cout << "Произведение: " << proizved << endl;
    }
    cout << "3)"<<endl;
    cout << "Отсортированный массив: " << endl;
    
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
