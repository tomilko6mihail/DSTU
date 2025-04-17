//
//  main.cpp
//  laba5
//
//  Created by Михаил on 17.04.2025.
//

#include <iostream>
#include <string>

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

float CircleS(float R){
    return 3.14 * R * 2;
}

bool equal(int N, int S) {
    if (N == 0) {
        return S == 0;
    }
    int lastDigit = N % 10;
    return equal(N / 10, S - lastDigit);
}

int calculateProduct(float* A, float* B, int N) {
    int productB = 1;
    for (int i = 0; i < N; i++) {
        productB *= B[i];
    }
    
    if (productB > 0) {
        int productA = 1;
        for (int i = 0; i < N; i++) {
            productA *= A[i];
        }
        return productA;
    } else {
        int sumA = 0, sumB = 0;
        for (int i = 0; i < N; i++) {
            sumA += A[i];
            sumB += B[i];
        }
        return sumA * sumB;
    }
}

int main(int argc, const char * argv[]) {
    
    met10:
    
    int k;
    cout << "----------МЕНЕДЖЕР ЗАДАНИЙ----------" << endl;
    cout << "Введите 1, чтобы открыть 1 задание" << endl;
    cout << "Введите 2, чтобы открыть 2 задание" << endl;
    cout << "Введите 3, чтобы открыть 3 задание" << endl;
    cout << "Номер задания: ";
    cin >>k;
    switch (k) {
        case 1:
        {
            for(int i = 0; i < 3; i++){
                cout << i + 1 << ")" << endl;
            met0:
                string strR;
                int R = 0;
                
                cout << "Введите радиус круга R: ";
                getline(cin, strR);
                if(validation(strR)){
                    R = stoi(strR);
                } else {
                    goto met0;
                }
                cout << "Длина круга = " << CircleS(R) << endl;
            }
            
            break;
        }
        case 2:
        {
        met1:
            string strN;
            int N = 0;
            
            cout << "Введите длину массива: ";
            getline(cin, strN);
            if(validation(strN)){
                N = stoi(strN);
            } else {
                goto met1;
            }
            
            float* arrA = new float[N];
            float* arrB = new float[N];
            
            cout << "Заполните массив A: " << endl;
            for(int i = 0; i < N; i++){
            met2:
                cout << "Элемент " << i + 1 << ": ";
                string numStr;
                cin >> numStr;
                if(validation(numStr)){
                    arrA[i] = stoi(numStr);
                } else {
                    goto met2;
                }
                
            }
            
            cout << "Заполните массив B: " << endl;
            for(int i = 0; i < N; i++){
            met3:
                cout << "Элемент " << i + 1 << ": ";
                string numStr;
                cin >> numStr;
                if(validation(numStr)){
                    arrB[i] = stoi(numStr);
                } else {
                    goto met3;
                }
                
            }
            
            cout << calculateProduct(arrA, arrB, N) << endl;
            
            break;
        }
        case 3:
        {
        met4:
            string strNumm, strS;
            int Numm, S = 0;
            
            cout << "Введите N: ";
            getline(cin, strNumm);
            if(validation(strNumm)){
                Numm = stoi(strNumm);
            } else {
                goto met4;
            }
            
        met5:
            cout << "Введите S: ";
            getline(cin, strS);
            if(validation(strS)){
                S = stoi(strS);
            } else {
                goto met5;
            }
            
            cout << equal(Numm, S) << endl;
            break;
        }
        default:
            cout << "Нет такого варианта." << endl;
            break;
    }
    goto met10;
}
