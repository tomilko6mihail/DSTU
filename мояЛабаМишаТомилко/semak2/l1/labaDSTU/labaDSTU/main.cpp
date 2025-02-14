#include <iostream>
#include <string>
using namespace std;

float find_Z(float x,  float y){
    return log10( sin(x*x) / (abs(cos(y)) + 2) );
}

float find_Y(float x){
    if(x > 7.7){
        return log(x) * sin(3 * x);
    }
    if(x == 7.7f){
        return pow(pow(x, 5) - 1, 1/5);
    }
    if(x < 7.7){
        return 1 + pow(cos(2*x), 3) - 3 * pow(sin(3 * x), 2);
    }
    return 0;
}
int main() {
    met1:
    float x, y;
    string xStr, yStr;
    cinx:
    cout << "Введите х: ";
    cin >> xStr;
    try{
        x = stof(xStr);
    }
    catch(invalid_argument){
        cout << "Некорректный формат данных!" << endl;
        goto cinx;
    }
    cout << x << endl;
    ciny:
    cout << "Введите у: ";
    cin >> yStr;
    try{
        y = stof(yStr);
    }
    catch(invalid_argument){
        cout << "Некорректный формат данных!" << endl;
        goto ciny;
    }
    cout << "Значение выражения для Z = " << find_Z(x, y) << endl;
    cout << "Значение выражения для Y = " << find_Y(x) << endl;
    goto met1;
}
