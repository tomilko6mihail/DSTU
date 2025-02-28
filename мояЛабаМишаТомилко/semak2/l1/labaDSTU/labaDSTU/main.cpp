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
    met1:
    float x, y;
    string xStr, yStr;
    cinx:
    cout << "Введите х: ";
    cin >> xStr;
    if(validation(xStr)){
        x = stof(xStr);
    }else{
        goto met1;
    }
    cout << x << endl;
    ciny:
    cout << "Введите у: ";
    cin >> yStr;
    if(validation(yStr)){
        y = stof(yStr);
    }else{
        goto met1;
    }
    cout << "Значение выражения для Z = " << find_Z(x, y) << endl;
    cout << "Значение выражения для Y = " << find_Y(x) << endl;
    goto met1;
}
///
  /// Мишка красавчик
/// Янаков в подвале
/// Ядерная установка Томилко-Янакова
/// Томифулин
/// Гарифуленко
/// 9 общага горит
/// С 23 февраля, любимые, подарка не будет, вас слишком много (в качестве подарка бомбим 9 общагу) (и 10 тоже)
///
///  но мы всё равно вас любим
///  но это не точно
///  например юру не любим
