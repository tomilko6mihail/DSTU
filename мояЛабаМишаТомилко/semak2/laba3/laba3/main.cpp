//#include <iostream>
//#include <random>
//#include <string>
//#include <cmath>
//
//using namespace std;
//
//bool validation(const string str){
//    if (str.empty()){
//        cout << "Строка не может быть пустой" << endl;
//        return false;
//    }
//    int startIndex = str[0] == '-' ? 1 : 0;
//    
//    for (int i = startIndex; i < str.length(); i++ ){
//        if(isspace(str[i])){
//            cout << "Число не должно содержать пробелы" << endl;
//            return false;
//        }
//        if (str[i] == '.'){
//            cout << "В числе не может быть точки" << endl;
//            return false;
//        }
//        else if (!isdigit(str[i])){
//            cout << "В числе не могут быть буквы" << endl;
//            return false;
//        }
//    }
//    if(startIndex < str.length()){
//        return true;
//    }
//    else{
//        cout << "Минус это не число" << endl;
//        return false;
//    }
//}
//
//int main() {
//    random_device rd;
//    mt19937 gen(rd());
//    uniform_real_distribution<> dis(-10.0, 10.0);
//    
//    met1:
//    string lenStr;
//    int len;
//    cout << "Введите размер массива: ";
//    getline(cin, lenStr);
//    
//    if(validation(lenStr)){
//        len = stoi(lenStr);
//    }else{
//        goto met1;
//    }
//    
//    if(len < 0 ){
//        cout << "Массив не может быть отрицательным" << endl;
//        goto met1;
//    }
//    
//    float *arr = new float[len];
//    
//    for(int i = 0; i < len; i++){
//        arr[i] = dis(gen);
//    }
//    
//    for(int i = 0; i < len; i++){
//        cout << arr[i] << " ";
//    }
//    
//    float min = 10.0;
//    int currentIdmin = 0;
//    float max = 0.0;
//    int currentIdmax = 0;
//    
//    for(int i = 0; i < len; i++){
//        if(fabs(arr[i]) < min){
//            min = fabs(arr[i]);
//            currentIdmin = i;
//        }
//        if(fabs(arr[i]) > max){
//            max = fabs(arr[i]);
//            currentIdmax = i;
//        }
//    }
//    
//    float proizved = 1.0;
//    
//    for(int i = currentIdmax + 1; i < len; i++){
//        proizved *= arr[i];
//    }
//
//    
//    for (int i = 0; i < len - 1; i++) {
//        for (int j = 0; j < len - i - 1; j++) {
//            if (arr[j] > arr[j + 1]) {
//                float temp = arr[j];
//                arr[j] = arr[j + 1];
//                arr[j + 1] = temp;
//            }
//        }
//    }
//    
//    cout << endl;
//    cout << "1)"<<endl;
//    cout << "Номер минимального по модулю числа: " << currentIdmin << endl;
//    cout << "Номер максимального по модулю числа: " << currentIdmax << endl;
//    cout << "2)"<<endl;
//    if(currentIdmax == len - 1){
//        cout << "После максимального элемента ничего нет" << endl;
//    } else {
//        cout << "Произведение: " << proizved << endl;
//    }
//    cout << "3)"<<endl;
//    cout << "Отсортированный массив: " << endl;
//    
//    for (int i = 0; i < len; i++) {
//        cout << arr[i] << " ";
//    }
//    cout << endl;
//}

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

bool validation(const string str) {
    if (str.empty()) {
        cout << "Строка не может быть пустой" << endl;
        return false;
    }
    int startIndex = str[0] == '-' ? 1 : 0;

    for (int i = startIndex; i < str.length(); i++) {
        if (isspace(str[i])) {
            cout << "Число не должно содержать пробелы" << endl;
            return false;
        }
        if (str[i] == '.') {
            cout << "В числе не может быть точки" << endl;
            return false;
        }
        else if (!isdigit(str[i])) {
            cout << "В числе не могут быть буквы" << endl;
            return false;
        }
    }
    if (startIndex < str.length()) {
        return true;
    }
    else {
        cout << "Минус это не число" << endl;
        return false;
    }
}


int main() {

    // Инициализация генератора случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10.0, 10.0);

    setlocale(0, ""); //для русского языка
met1:
    int len;
    string lenStr;
    cout << "Размер массива для кода арины четсно: ";
    getline(cin, lenStr);
    //проверка данных
    if (validation(lenStr)) {
        len = stoi(lenStr);
    }
    else {
        goto met1;
    }
    if (len < 0) {
        cout << "Массив не может быть отрицательным" << endl;
        goto met1;
    }

    // Выделение памяти для массива
    float* arr = new float[len];
    if (arr == nullptr) { // Проверка на успешность выделения памяти
        cerr << "Ошибка при выделении памяти!" << std::endl;
        return 1; // Выход при ошибке
    }

    // Заполнение массива случайными числами
    for (int i = 0; i < len; i++) {
        arr[i] = dis(gen);
    }

    //вывод массива
    cout << "Array: " << endl;
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    //произведение положительных элементов массива
    float composition = 1.0;

    for (int i = 0; i < len; i++) {
        if (arr[i] > 0) {
            composition *= arr[i];
        }
    }

    //находим минимальное число в массиве и его айдишник
    float min = 10.0;
    int currentIdmin = 0;

    for (int i = 0; i < len; i++) {
        if (arr[i] < min) {
            min = arr[i];
            currentIdmin = i;
        }
    }


    //сумма элементов массива до минимального
    float summ = 0.0;

    for (int i = 0; i < currentIdmin; i++) {
        summ += arr[i];
    }

    float *even = new float[len/ 2 + 1];  // Массив для четных индексов (допустим, что размер четен)
    float *odd = new float[len/ 2];        // Массив для нечетных индексов

    int evenCount = 0, oddCount = 0;

    // Извлечение элементов на четных и нечетных местах
    for (int i = 0; i < len; ++i) {
        if (i % 2 == 0) {
            even[evenCount++] = arr[i];
        }
        else {
            odd[oddCount++] = arr[i];
        }
    }

    // Сортировка четных и нечетных элементов
    sort(even, even + evenCount);
    sort(odd, odd + oddCount);

    // Слияние отсортированных элементов обратно в исходный массив
    evenCount = 0; // Сброс счетчика
    oddCount = 0;  // Сброс счетчика

    for (int i = 0; i < len; ++i) {
        if (i % 2 == 0) {
            arr[i] = even[evenCount++];
        }
        else {
            arr[i] = odd[oddCount++];
        }
    }

    cout << "Произведение положительных чисел в массиве: " << composition << endl;
    cout << "Сумма элементов до минимального: " << summ << endl;
    cout << "Отсортированный массив: ";
    for (int i = 0; i < len; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    

    cout << endl;
    delete[] arr;
    delete[] even;
    delete[] odd;

    return 0;
}
