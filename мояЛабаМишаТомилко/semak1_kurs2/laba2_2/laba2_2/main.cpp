//#include <iostream>
//#include <vector>
//#include <string>
//
//using namespace std;
//
//class Ticket {
//private:
//    string movie;
//    string date;
//    double price;
//    int seats;
//    int maxSeats = 120;
//
//public:
//
//    Ticket() : movie(""), date(""), price(0), seats(0) {}
//    Ticket(string m, string d, double p, int s) : movie(m), date(d), price(p), seats(s) {}
//    
//    Ticket(const Ticket& other) : movie(other.movie), date(other.date), price(other.price), seats(other.seats) {}
//
//    string getMovie() { return movie; }
//    string getDate() { return date; }
//    double getPrice() { return price; }
//    int getSeats() { return seats; }
//    int getMaxSeats() { return maxSeats; }
//
//    void setMovie(string m) { movie = m; }
//    void setDate(string d) { date = d; }
//    void setPrice(double p) { price = p; }
//    void setSeats(int s) { seats = s; }
//
//    double getRevenue() {
//        return price * seats;
//    }
//    
//    void show() {
//        cout << movie << " - " << date << " - " << seats << " мест - " << getRevenue() << " руб.\n";
//    }
//};
//
//int main() {
//    vector<Ticket> tickets;
//    
//    tickets.push_back(Ticket("Аватар 1 xfcnm", "2024-01-15", 350, 120));
//    tickets.push_back(Ticket("Интерстеллар", "2024-01-16", 300, 150));
//    tickets.push_back(Ticket("Аватар 2 xfcnm", "2024-01-17", 350, 95));
//    tickets.push_back(Ticket("Начало", "2024-01-18", 250, 200));
//    
//    cout << "Все билеты:\n";
//    for(auto& t : tickets) {
//        t.show();
//    }
//    
//    double total = 0;
//    for(auto& t : tickets) {
//        total += t.getRevenue();
//    }
//    cout << "\nОбщая выручка: " << total << " руб.\n";
//    
//    // Фильмы за период
//    string start = "2024-01-15";
//    string end = "2024-01-17";
//    cout << "\nФильмы с " << start << " по " << end << ":\n";
//    
//    for(auto& t : tickets) {
//        cout <<"--------------------------------"<<endl;
//        if(t.getMaxSeats() < t.getSeats()){
//            cout << "Продано билетов больше, чем есть мест" << endl;
//        }
//        if(t.getDate() >= start && t.getDate() <= end) {
//            cout << "- " << t.getMovie() << " (" << t.getDate() << ")\n";
//        }
//        cout <<"--------------------------------"<<endl;
//    }
//    
//    return 0;
//}

#include <iostream>
#include <vector>

using namespace std;

//Наследование
//
//class Human {
//public:
//    int age;
//    void sayHi(){
//        cout << "hi, my age: " << age;
//    }
//};
//
//class Student : public Human {
//public:
//    string name;
//    void speak(){
//        sayHi();
//        cout << ", and my name is " << name;
//    }
//};
//
//int main(){
//    Student s;
//    s.name = "grisha";
//    s.age = 21;
//    s.speak();
//}


//Композиция
//class Engine {
//public:
//    int power;
//    Engine(int p) : power(p) {}
//};
//
//class Car {
//public:
//    Engine engine;
//    Car(int power) : engine(power) {}
//};

//Агрегация
//class Teacher {
//public:
//    int age;
//    Teacher(int a) : age(a) {}
//};
//
//class School {
//    vector<Teacher*> teachers;
//    void addTeacher(Teacher* t){
//        teachers.push_back(t);
//    }
//};

//Полиморфизм, наследование, виртуальная функция
//class Shape{
//public:
//    virtual double calcS() const = 0;
//};
//
//class Circle : public Shape {
//public:
//    int radius;
//    Circle(int r) : radius(r) {}
//    virtual double calcS() const override {
//        return radius * 3.14;
//    }
//};
//
//int main(){
//    Circle circle(13);
//}
