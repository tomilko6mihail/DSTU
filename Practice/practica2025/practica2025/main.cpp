#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <matplot/matplot.h>

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

struct point {
    double x, y;
    int name;
};

double euq_distance(point a, point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<point> read_points(const string& filename, bool isKmean) {
    vector<point> points;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return points;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        point p;
        if (isKmean) {
            if (iss >> p.x >> p.y) {
                p.name = -1;
                points.push_back(p);
            } else {
                cerr << "Ошибка чтения строки: " << line << endl;
            }
        } else {
            if (iss >> p.x >> p.y >> p.name) {
                points.push_back(p);
            } else {
                cerr << "Ошибка чтения строки: " << line << endl;
            }
        }
    }

    file.close();
    return points;
}

vector<point> dataNearest = read_points("dataPointsKNN.txt", false);
vector<point> dataClasters = read_points("dataPointsKmeans.txt", true);
vector<point> centroidsData;
vector<double> x, y, c;
vector<double> xCentroid, yCentroid, cCentroid;

void makePoints(const vector<point>& data){
    x.clear();
    y.clear();
    c.clear();
    for (const auto& p : data) {
        x.push_back(p.x);
        y.push_back(p.y);
        c.push_back(static_cast<double>(p.name));
    }
}

void makeCentroids(const vector<point>& data){
    xCentroid.clear();
    yCentroid.clear();
    cCentroid.clear();
    for (const auto& p : data) {
        xCentroid.push_back(p.x);
        yCentroid.push_back(p.y);
        cCentroid.push_back(static_cast<double>(p.name));
    }
}

int kNearest(const vector<point>& data, point newPoint, int k) {
    vector<pair<int, int>> distances;
    
    for (const auto& p : data) {
        int dist = euq_distance(p, newPoint);
        distances.push_back({dist, p.name});
    }
    
    sort(distances.begin(), distances.end());
    
    cout << "Ближайшие точки(0 - пицца, 1 - бургер): ";
    for (const auto& p : distances) {
        cout<<p.second<<" ";
    }
    
    int count_name_classes[2] = {0};
    if(k > data.size()){
        k = static_cast<int>(data.size());
    }
    for (int i = 0; i < k; i++) {
        count_name_classes[distances[i].second]++;
    }
    

    return count_name_classes[0] > count_name_classes[1] ? 0 : 1;
}

void kMeans(vector<point>& data, int k, int maxIterations) {

    vector<point> centroids(k);
    srand(time(0));
    for (int i = 0; i < k; i++) {
        int idx = rand() % data.size();
        centroids[i] = {data[idx].x, data[idx].y, i};
    }

    for (int iter = 0; iter < maxIterations; iter++) {
        for (auto& c : centroids){
            cout << "Центроид " << c.name << " сейчас в координате (" << c.x << ", " << c.y << ")" << endl;
        }

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
        
        vector<int> count_of_points(k, 0);
        vector<double> sumX(k, 0), sumY(k, 0);
        for (const auto& p : data) {
            sumX[p.name] += p.x;
            sumY[p.name] += p.y;
            count_of_points[p.name]++;
        }
        for (int i = 0; i < k; i++) {
            if (count_of_points[i] > 0) {
                centroids[i].x = sumX[i] / count_of_points[i];
                centroids[i].y = sumY[i] / count_of_points[i];
            }
        }
        centroidsData = centroids;
    }
    
    for (const auto& p : data) {
        cout << "Точка (" << p.x << ", " << p.y << ") принадлежит классу " << p.name + 1 << endl;
    }
    dataClasters = data;
}

int main() {
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
            point newPoint = {static_cast<double>(d), static_cast<double>(kal), -1};
            
            int predictedClass = kNearest(dataNearest, newPoint, k);
            string answer = predictedClass == 0 ? "пицца." : "бургер.";
            cout << endl << "Скорее всего, это " << answer << endl;
            
            met2:
            cout << "|=-= Сохранить вашу точку в бд? " << endl;
            cout << "|=-= Нажмите 1, если да" << endl;
            cout << "|=-= Нажмите 2, если нет " << endl;
            int saveOrNo = tryInt("|=-= Ваш выбор: ");
            
            switch (saveOrNo) {
                case 1:
                {
                    ofstream file("dataPointsKNN.txt", ios::app);
                    if (!file.is_open()) {
                        cout << "Ошибка: не удалось открыть файл" << endl;
                        break;
                    }
                    file << d << " " << kal << " " << predictedClass << "\n";
                    if (!file) {
                        cerr << "|=-= Ошибка записи! " << endl;
                    }
                    else {
                        cerr << "|=-= Ваша точка записана! " << endl;
                    }
                    file.close();
                    break;
                }
                case 2:
                {
                    break;
                }
                default:
                {
                    cout << "|=-= Введите число от 1 до 2! =--------------=|" << endl;
                    cout << "|=-------------------------------------------=|" << endl;
                    goto met2;
                    break;
                }
            }
            
            cout << "|=-= Визуализировать результат? " << endl;
            cout << "|=-= Нажмите 1, если да " << endl;
            cout << "|=-= Нажмите 2, если нет (возврат в меню) " << endl;
            int swiVisual = tryInt("|=-= Ваш выбор: ");
            
            switch (swiVisual) {
                case 1:
                {
                    makePoints(dataNearest);
                    matplot::grid(true);
                    matplot::scatter(x, y, 7.0, c)->marker_style(".");
                    matplot::hold(true);
                    matplot::scatter(vector{d}, vector{kal}, 10.0, vector{predictedClass})->marker_style("x");
                    matplot::hold(false);
                    matplot::colormap(matplot::palette::viridis());
                    matplot::colorbar().label("Класс");
                    matplot::title("Метод k-ближайших соседей");
                    matplot::xlabel("Диаметр (см)");
                    matplot::ylabel("Калорийность (ккал)");
                    matplot::show();
                    break;
                }
                case 2:
                {
                    goto met1;
                    break;
                }
                default:
                {
                    cout << "|=-= Введите число от 1 до 2! =--------------=|" << endl;
                    cout << "|=-------------------------------------------=|" << endl;
                    goto met1;
                    break;
                }
            }
            
            break;
        }
        case 2:
        {
            int k = tryInt("Введите количество кластеров: ", "Число должно быть больше или равно 1.", 1);
            kMeans(dataClasters, k, 10);
            cout << "|=-= Визуализировать? " << endl;
            cout << "|=-= Нажмите 1, если да " << endl;
            cout << "|=-= Нажмите 2, если нет (возврат в меню) " << endl;
            int swiVisual = tryInt("|=-= Ваш выбор: ");
            
            switch (swiVisual) {
                case 1:
                {
                    makePoints(dataClasters);
                    makeCentroids(centroidsData);
                    matplot::grid(true);
                    matplot::scatter(x, y, 7.0, c)->marker_style(".");
                    matplot::hold(true);
                    matplot::scatter(xCentroid, yCentroid, 10.0, cCentroid)->marker_style("x");
                    matplot::hold(false);
                    matplot::colormap(matplot::palette::viridis());
                    matplot::colorbar().label("Класс");
                    matplot::title("Метод k-средних соседей");
                    matplot::xlabel("Ось х");
                    matplot::ylabel("Ось у");
                    matplot::show();
                    break;
                }
                case 2:
                {
                    goto met1;
                    break;
                }
                default:
                {
                    cout << "|=-= Введите число от 1 до 2! =--------------=|" << endl;
                    cout << "|=-------------------------------------------=|" << endl;
                    goto met1;
                    break;
                }
            }
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
