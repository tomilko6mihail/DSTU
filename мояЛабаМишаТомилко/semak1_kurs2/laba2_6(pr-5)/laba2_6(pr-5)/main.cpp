#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class CompanyEmployee {
protected:
    string name;
    int age;
    int accessLevel;
    string position;
public:
    CompanyEmployee(const string& n, int a, int al, const string& p)
        : name(n), age(a), accessLevel(al), position(p) {}
    
    virtual ~CompanyEmployee() = default;
    
    virtual void work() const = 0;
    
    void setAge(int a) { age = a; }
    int getAge() const { return age; }
    
    void setAccessLevel(int al) { accessLevel = al; }
    int getAccessLevel() const { return accessLevel; }
    
    void setPosition(const string& p) { position = p; }
    string getPosition() const { return position; }
    
    string getName() const { return name; }
    
    virtual void displayInfo() const {
        cout << "Имя: " << name << "\n";
        cout << "Возраст: " << age << "\n";
        cout << "Уровень допуска: " << accessLevel << "\n";
        cout << "Должность: " << position << "\n";
    }
};

class Intern : public CompanyEmployee {
private:
    string mentor;
    int internshipDuration;
public:
    Intern(const string& n, int a, int al, const string& p,
           const string& m, int duration)
        : CompanyEmployee(n, a, al, p), mentor(m), internshipDuration(duration) {}
    
    void work() const override {
        cout << name << " проходит стажировку под руководством " << mentor << "\n";
    }
    
    void displayInfo() const override {
        CompanyEmployee::displayInfo();
        cout << "Наставник: " << mentor << "\n";
        cout << "Длительность стажировки: " << internshipDuration << " месяцев\n";
        cout << "---\n";
    }
    
    void setMentor(const string& m) { mentor = m; }
    string getMentor() const { return mentor; }
};

class Manager : public CompanyEmployee {
private:
    string department;
    int subordinatesCount;
public:
    Manager(const string& n, int a, int al, const string& p,
            const string& dep, int subs)
        : CompanyEmployee(n, a, al, p), department(dep), subordinatesCount(subs) {}
    
    void work() const override {
        cout << name << " управляет отделом " << department << "\n";
    }
    
    void displayInfo() const override {
        CompanyEmployee::displayInfo();
        cout << "Отдел: " << department << "\n";
        cout << "Количество подчиненных: " << subordinatesCount << "\n";
        cout << "---\n";
    }
    
    void conductMeeting() const {
        cout << name << " проводит совещание в отделе " << department << "\n";
    }
    
    void setDepartment(const string& dep) { department = dep; }
    string getDepartment() const { return department; }
};

class Director : public CompanyEmployee {
private:
    string companyCar;
    double budget;
public:
    Director(const string& n, int a, int al, const string& p,
             const string& car, double b)
        : CompanyEmployee(n, a, al, p), companyCar(car), budget(b) {}
    
    void work() const override {
        cout << name << " управляет всей компанией с бюджетом " << budget << "\n";
    }
    
    void displayInfo() const override {
        CompanyEmployee::displayInfo();
        cout << "Служебный автомобиль: " << companyCar << "\n";
        cout << "Бюджет: " << budget << " руб.\n";
        cout << "---\n";
    }
    
    void makeStrategicDecision() const {
        cout << name << " принимает стратегическое решение\n";
    }
    
    void setBudget(double b) { budget = b; }
    double getBudget() const { return budget; }
};

class Company {
private:
    vector<unique_ptr<CompanyEmployee>> employees;
public:
    void addEmployee(unique_ptr<CompanyEmployee> employee) {
        employees.push_back(move(employee));
    }
    
    void displayAllEmployees() const {
        cout << "=== Все сотрудники компании ===\n";
        for (const auto& emp : employees) {
            emp->displayInfo();
        }
    }
    
    void makeEveryoneWork() const {
        cout << "=== Работа сотрудников ===\n";
        for (const auto& emp : employees) {
            emp->work();
        }
        cout << "---\n";
    }
    
    void findEmployeesByAge(int targetAge) const {
        cout << "Сотрудники возраста " << targetAge << ":\n";
        for (const auto& emp : employees) {
            if (emp->getAge() == targetAge) {
                cout << emp->getName() << " - " << emp->getPosition() << "\n";
            }
        }
        cout << "---\n";
    }
};

class Dushnila {
public:
    virtual void work(){
        cout << "saying";
    };
};

class worker : public Dushnila{
public:
    void work() override {
        cout << "очень душно душнит";
    }
};

int main() {
    Company company;
    
    company.addEmployee(make_unique<Intern>("Анна Петрова", 22, 1, "Стажер",
                                           "Иван Сидоров", 6));
    company.addEmployee(make_unique<Manager>("Иван Сидоров", 35, 3, "Менеджер",
                                            "IT", 8));
    company.addEmployee(make_unique<Director>("Сергей Козлов", 45, 5, "Директор",
                                             "Mercedes S-Class", 5000000.0));
    company.addEmployee(make_unique<Intern>("Дмитрий Волков", 24, 1, "Стажер",
                                           "Мария Иванова", 3));
    company.addEmployee(make_unique<Manager>("Мария Иванова", 40, 4, "Старший менеджер",
                                            "Маркетинг", 12));
    
    company.displayAllEmployees();
    company.makeEveryoneWork();
    company.findEmployeesByAge(35);
    
    return 0;
}
