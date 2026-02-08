//#include <iostream>
//using namespace std;
//
//template<typename T>
//T getMax(T a, T b) {
//    return (a > b) ? a : b;
//}
////шаб функции искать имена собственные в каком то текстовом файле и там будет разделение на мужские и женские имена
//template<typename T>
//class NumberPair {
//private:
//    T first;
//    T second;
//public:
//    NumberPair(T a, T b) : first(a), second(b) {}
//    
//    T getSum() {
//        return first + second;
//    }
//};
//
//template<>
//char getMax<char>(char a, char b) {
//    return (a > b) ? a : b;
//}
//
//template<>
//class NumberPair<float> {
//private:
//    float first;
//    float second;
//public:
//    NumberPair(float a, float b) : first(a), second(b) {}
//    
//    float getSum() {
//        return first + second;
//    }
//    
//    float getDifference() {
//        return first - second;
//    }
//};
//
//int main() {
//    cout << getMax(5, 3) << endl;
//    cout << getMax(2.7, 3.1) << endl;
//    cout << getMax('a', 'z') << endl;
//    
//    NumberPair<int> intPair(10, 20);
//    cout << intPair.getSum() << endl;
//    
//    NumberPair<float> floatPair(5.5f, 2.2f);
//    cout << floatPair.getSum() << endl;
//    cout << floatPair.getDifference() << endl;
//    
//    return 0;
//}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

template<typename Container>
class NameFinder {
private:
    Container maleNames;
    Container femaleNames;
    
    bool contains(const Container& container, const string& name) const {
        return container.find(name) != container.end();
    }
    
public:
    NameFinder(const Container& male, const Container& female)
        : maleNames(male), femaleNames(female) {}
    
    pair<bool, bool> findName(const string& word) const {
        bool isMale = contains(maleNames, word);
        bool isFemale = contains(femaleNames, word);
        return {isMale, isFemale};
    }
    
    void processFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл: " << filename << endl;
            return;
        }
        
        string word;
        while (file >> word) {
            auto [isMale, isFemale] = findName(word);
            
            if (isMale && isFemale) {
                cout << word << " - может быть как мужским, так и женским именем" << endl;
            } else if (isMale) {
                cout << word << " - мужское имя" << endl;
            } else if (isFemale) {
                cout << word << " - женское имя" << endl;
            }
        }
        file.close();
    }
};

int main() {
    unordered_set<string> maleNames = {"Александр", "Иван", "Петр", "Михаил", "Сергей"};
    unordered_set<string> femaleNames = {"Анна", "Мария", "Елена", "Ольга", "Наталья"};
    
    NameFinder<unordered_set<string>> finder(maleNames, femaleNames);
    
    finder.processFile("text.txt");
    
    return 0;
}
