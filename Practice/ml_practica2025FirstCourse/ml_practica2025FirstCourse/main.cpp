#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
//#include "gnuplot-iostream.h"

using namespace std;

bool validationInt(const string str) {
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

int tryInt(string title, string error = "", int minNumber = -999999999){
    string str;
    int strNumber;
    
    while (true) {
        cout << title;
        string str;
        getline(cin, str);
        
        if (validationInt(str)) {
            strNumber = stoi(str);
            if(strNumber < minNumber) {
                cout << error << endl;
            } else {
                return strNumber;
            }
        }
    }
}


// Структура для точки с координатами и классом
struct point {
    int x, y; // Координаты точки
    int name;   // Метка класса (например, 0 или 1)
};

// Функция для вычисления евклидова расстояния
double euq_distance(point a, point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Метод k-NN для предсказания класса новой точки
int kNearest(const vector<point>& data, point newPoint, int k) {
    vector<pair<int, int>> distances;
    
    // Вычисляем расстояния до всех точек
    for (const auto& p : data) {
        int dist = euq_distance(p, newPoint);
        distances.push_back({dist, p.name});
    }
    
    // Сортируем по расстоянию
    sort(distances.begin(), distances.end());
    
    cout << "Ближайшие точки(0 - пицца, 1 - бургер): ";
    for (const auto& p : distances) {
        cout<<p.second<<" ";
    }
    
    // Подсчитываем голоса k ближайших соседей
    int count_name_classes[2] = {0}; // Предполагаем два класса: 0 и 1
    if(k > data.size()){
        k = static_cast<int>(data.size()); //потому что возвращает size_t
    }
    for (int i = 0; i < k; i++) {
        count_name_classes[distances[i].second]++;
    }
    
    // Возвращаем класс с большинством голосов
    return count_name_classes[0] > count_name_classes[1] ? 0 : 1;
}

// Метод k-средних
void kMeans(vector<point>& data, int k, int maxIterations) {
    // Инициализация центроидов случайным образом
    vector<point> centroids(k);
    srand(time(0));
    for (int i = 0; i < k; i++) {
        int idx = rand() % data.size();
        centroids[i] = {data[idx].x, data[idx].y, i};
    }
    
    // Итерации k-средних
    for (int iter = 0; iter < maxIterations; iter++) {
        for (auto& c : centroids){
            cout << "Центроид " << c.name << " сейчас в координате (" << c.x << ", " << c.y << ")" << endl;
        }
        // Назначение точек кластерам
        for (auto& p : data) {
            double minDist = euq_distance(p, centroids[0]);
            p.name = 0;
            for (int i = 1; i < k; i++) {
                double dist = euq_distance(p, centroids[i]);
                if (dist < minDist) {
                    minDist = dist;
                    p.name = i;
                }
            }
        }
        
        // Обновление центроидов
        vector<int> count_of_points(k, 0); // количество элементов этого векотра будут соответсовать количеству кластеров и хранить в себе число точек в них
        vector<double> sumX(k, 0), sumY(k, 0); // столько же сумм координат, сколько и кластеров
        for (const auto& p : data) {
            sumX[p.name] += p.x;
            sumY[p.name] += p.y;
            count_of_points[p.name]++; // увеличиваем количество точек в определенном кластере на 1 (добавляем точку)
        }
        for (int i = 0; i < k; i++) {
            if (count_of_points[i] > 0) {
                centroids[i].x = sumX[i] / count_of_points[i]; // среднее арифметическое координаты х для кластера
                centroids[i].y = sumY[i] / count_of_points[i]; // среднее арифметическое координаты у для кластера
            }
        }
    }
    
    // Вывод результатов
    for (const auto& p : data) {
        cout << "Точка (" << p.x << ", " << p.y << ") принадлежит классу " << p.name + 1 << endl;
    }
}

vector<point> dataClasters = {
    {1, 1, -1}, {2, 1, -1}, {1, 2, -1},
    {5, 5, -1}, {6, 5, -1}, {5, 6, -1},
    {6, 2, -1}, {6, 3, -1}, {7, 2, -1}
};

vector<point> dataNearest = {
    {30, 800, 0},
    {35, 1200, 0},
    {28, 950, 0},
    {15, 300, 0},
    {10, 350, 1},
    {12, 600, 1},
    {8, 200, 1},
    {20, 900, 1},
    {18, 500, 0},
    {9, 450, 1}
};

int main() {
//    Gnuplot gp;
//        std::vector<std::pair<double, double>> points1 = {
//            {1.0, 1.0}, {2.0, 1.5}, {1.5, 2.0},
//            {5.0, 5.0}, {6.0, 5.5}, {5.5, 6.0}
//        };
//        gp << "set style data points\n";
//        gp << "plot '-' with points pt 7 ps 2\n";
//        gp.send1d(points1); // Отправляем контейнер точек
//    
    met1:
    cout << "|=----------------+=========+----------------=|" << endl;
    cout << "|=----------------| М Е Н Ю |----------------=|" << endl;
    cout << "|=----------------+=========+----------------=|" << endl;
    cout << "|=-------------------------------------------=|" << endl;
    cout << "|=-= Нажмите 1 для k-ближайших соседей =-----=|" << endl;
    cout << "|=-= Нажмите 2 для k-средних соседей =-------=|" << endl;
    cout << "|=-= Нажмите 3 для выхода =------------------=|" << endl;
    int swi = tryInt("|=-= Ваш выбор: ");
    
    switch (swi) {
        case 1:
        {
            int d = tryInt("Введите диаметр блюда: ", "Это и не пицца, и не бургер. Введите диаметр, больший 5.", 5);
            int kal = tryInt("Введите калорийность блюда: ", "Слишком маленькая калорийность. Введите число, большее 200.", 200);
            int k = tryInt("Введите количество ближайших соседей для точки: ", "Число должно быть больше или равно двум.", 2);
            point newPoint = {d, kal, -1};
            
            int predictedClass = kNearest(dataNearest, newPoint, k);
            string answer = predictedClass == 0 ? "пицца." : "бургер.";
            cout << endl << "Скорее всего, это " << answer << endl;
            goto met1;
            break;
        }
        case 2:
        {
            int k = tryInt("Введите количество кластеров: ", "Число должно быть больше или равно 1.", 1);
            kMeans(dataClasters, k, 10);
            goto met1;
            break;
        }
        case 3:
        {
            return 0;
            break;
        }
        default:
            cout << "|=-= Введите число от 1 до 3! =--------------=|" << endl;
            cout << "|=-------------------------------------------=|" << endl;
            goto met1;
            break;
    }
    
    return 0;
}
