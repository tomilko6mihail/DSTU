#include <iostream>
#include <cstring>

class Person {
private:
    char* name;
    int age;
    bool gender;
    
    Person() : name(nullptr), age(0), gender(true) {}
    Person(const char* name, int age, bool gender) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->age = age;
        this->gender = gender;
    }
    
    ~Person() {
        delete[] name;
    }
    
    static int objectCount;
    
public:
    static Person* createPerson() {
        objectCount++;
        checkOMON();
        return new Person();
    }
    
    static Person* createPerson(const char* name, int age, bool gender) {
        objectCount++;
        checkOMON();
        return new Person(name, age, gender);
    }
    
    static void destroyPerson(Person* person) {
        if (person) {
            objectCount--;
            delete person;
        }
    }
    
    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 150) {
            age = newAge;
        }
    }
    
    int getAge() const {
        return age;
    }
    
    void setName(const char* newName) {
        delete[] name;
        name = new char[strlen(newName) + 1];
        strcpy(name, newName);
    }
    
    const char* getName() const {
        return name ? name : "Unknown";
    }
    
    void setGender(bool newGender) {
        gender = newGender;
    }
    
    bool getGender() const {
        return gender;
    }
    
    const char* getGenderString() const {
        return gender ? "Male" : "Female";
    }
    
    void displayInfo() const {
        std::cout << "Name: " << getName()
                  << ", Age: " << age
                  << ", Gender: " << getGenderString() << std::endl;
    }
    
    static int getObjectCount() {
        return objectCount;
    }
    
    static void checkOMON() {
        if (objectCount > 10) {
            std::cout << "ОМОН ВЫЕХАЛ! Слишком много людей (" << objectCount
                      << ")! Толпа разгоняется!" << std::endl;
        }
        else if (objectCount == 10) {
            std::cout << "внимание! Достигнут предел в 10 человек. ОМОН наготове!" << std::endl;
        }
    }
    
    static void forceOMONCheck() {
        std::cout << "Проверка безопасности..." << std::endl;
        checkOMON();
    }
    
    friend void celebrateBirthday(Person& person);
};

int Person::objectCount = 0;

void celebrateBirthday(Person& person) {
    person.age++;
    std::cout << "Happy Birthday, " << person.getName() << "! Now you are "
              << person.age << " years old!" << std::endl;
}

Person* createPersonExtern(const char* name, int age, bool gender) {
    return Person::createPerson(name, age, gender);
}

void destroyPersonExtern(Person* person) {
    Person::destroyPerson(person);
}

void testArrayCreation() {
    std::cout << "\n=== Testing array creation ===" << std::endl;
    std::cout << "Object count before array: " << Person::getObjectCount() << std::endl;

    const int ARRAY_SIZE = 5;
    Person* people[ARRAY_SIZE];
    
    const char* names[] = {"Alice", "Bob", "Charlie", "David", "Eva"};
    int ages[] = {25, 30, 35, 28, 22};
    bool genders[] = {false, true, true, true, false};
    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        people[i] = Person::createPerson(names[i], ages[i], genders[i]);
        std::cout << "Создан: " << names[i] << " | Всего людей: " << Person::getObjectCount() << std::endl;
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        people[i]->displayInfo();
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        Person::destroyPerson(people[i]);
        std::cout << "Удален: " << names[i] << " | Осталось людей: " << Person::getObjectCount() << std::endl;
    }
    
    std::cout << "Object count after array destruction: " << Person::getObjectCount() << std::endl;
}

void testOMONScenario() {
    std::cout << "\n=== Testing OMON scenario ===" << std::endl;
    std::cout << "Object count before OMON test: " << Person::getObjectCount() << std::endl;

    const int CROWD_SIZE = 12;
    Person* crowd[CROWD_SIZE];
    
    std::cout << "Создаем толпу из " << CROWD_SIZE << " человек..." << std::endl;
    
    for (int i = 0; i < CROWD_SIZE; ++i) {
        char name[20];
        sprintf(name, "Person%d", i + 1);
        crowd[i] = Person::createPerson(name, 20 + i, i % 2 == 0);
        std::cout << "Создан " << name << " | Всего: " << Person::getObjectCount() << std::endl;
    }

    Person::forceOMONCheck();

    std::cout << "\n--- ОМОН начинает разгон толпы ---" << std::endl;
    for (int i = 0; i < CROWD_SIZE; ++i) {
        Person::destroyPerson(crowd[i]);
        std::cout << "Удален Person" << (i + 1) << " | Осталось: " << Person::getObjectCount() << std::endl;
    }

    std::cout << "Object count after OMON operation: " << Person::getObjectCount() << std::endl;
}

void testDynamicCreation() {
    std::cout << "\n=== Testing dynamic creation ===" << std::endl;
    std::cout << "Object count before dynamic creation: " << Person::getObjectCount() << std::endl;

    Person* dynamicPerson = Person::createPerson("Diana", 28, false);
    std::cout << "Object count after dynamic creation: " << Person::getObjectCount() << std::endl;
    
    dynamicPerson->displayInfo();

    std::cout << "Changing age..." << std::endl;
    dynamicPerson->setAge(29);
    dynamicPerson->displayInfo();
    
    celebrateBirthday(*dynamicPerson);
    dynamicPerson->displayInfo();
    
    Person::destroyPerson(dynamicPerson);
    std::cout << "Object count after dynamic destruction: " << Person::getObjectCount() << std::endl;
}

void testExternalFunctions() {
    std::cout << "\n=== Testing external functions ===" << std::endl;
    std::cout << "Object count before external creation: " << Person::getObjectCount() << std::endl;
    
    Person* externalPerson = createPersonExtern("Eve", 22, false);
    std::cout << "Object count after external creation: " << Person::getObjectCount() << std::endl;
    
    externalPerson->displayInfo();
    
    destroyPersonExtern(externalPerson);
    std::cout << "Object count after external destruction: " << Person::getObjectCount() << std::endl;
}

int main() {
    std::cout << "=== Person Class Test Program with OMON ===" << std::endl;
    std::cout << "Object count at main start: " << Person::getObjectCount() << std::endl;
    
    testArrayCreation();
    testDynamicCreation();
    testExternalFunctions();
    testOMONScenario();

    std::cout << "\n=== Testing exactly 10 people ===" << std::endl;
    Person* exactly10[10];
    for (int i = 0; i < 10; ++i) {
        char name[20];
        sprintf(name, "TestPerson%d", i + 1);
        exactly10[i] = Person::createPerson(name, 25, true);
    }
    

    std::cout << "\nПопытка создать 11-го человека..." << std::endl;
    Person* eleventh = Person::createPerson("Eleventh", 30, false);
    
    for (int i = 0; i < 10; ++i) {
        Person::destroyPerson(exactly10[i]);
    }
    if (eleventh) {
        Person::destroyPerson(eleventh);
    }
    
    std::cout << "\n=== Final Results ===" << std::endl;
    std::cout << "Final object count: " << Person::getObjectCount() << std::endl;
    
    return 0;
}
