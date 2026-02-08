//#include <iostream>
//#include <string>
//#include <vector>
//
//using namespace std;
//
//class Syllabus {
//private:
//    string content;
//public:
//    Syllabus(const string& c) : content(c) {}
//    string getContent() const { return content; }
//};
//
//class Student {
//private:
//    string name;
//public:
//    Student(const string& n) : name(n) {}
//    string getName() const { return name; }
//};
//
//class Course {
//private:
//    string title;
//    string description;
//    int duration;
//    string teacher;
//    Syllabus* syllabus;
//    vector<Student*> students;
//public:
//    Course(const string& t, const string& d, int dur, const string& teach, Syllabus* s)
//        : title(t), description(d), duration(dur), teacher(teach), syllabus(s) {}
//    
//    void addStudent(Student* student) {
//        students.push_back(student);
//    }
//    
//    string getTitle() const { return title; }
//    string getTeacher() const { return teacher; }
//    vector<Student*> getStudents() const { return students; }
//    
//    void display() const {
//        cout << "Курс: " << title << "\nПреподаватель: " << teacher
//                  << "\nДлительность: " << duration << " часов\n";
//        if (syllabus) {
//            cout << "Программа: " << syllabus->getContent() << "\n";
//        }
//        cout << "Студенты: ";
//        for (const auto& student : students) {
//            cout << student->getName() << " ";
//        }
//        cout << "\n\n";
//    }
//};
//
//class Curriculum {
//private:
//    vector<Course*> courses;
//public:
//    void addCourse(Course* course) {
//        courses.push_back(course);
//    }
//    
//    vector<Course*> getCourses() const { return courses; }
//};
//
//class EducationalPlatform {
//private:
//    vector<Curriculum*> curricula;
//public:
//    void addCurriculum(Curriculum* curriculum) {
//        curricula.push_back(curriculum);
//    }
//    
//    void displayAllCourses() {
//        cout << "Все курсы:\n";
//        for (const auto& curriculum : curricula) {
//            for (const auto& course : curriculum->getCourses()) {
//                course->display();
//            }
//        }
//    }
//    
//    void searchByTeacher(const string& teacher) {
//        cout << "Курсы преподавателя " << teacher << ":\n";
//        for (const auto& curriculum : curricula) {
//            for (const auto& course : curriculum->getCourses()) {
//                if (course->getTeacher() == teacher) {
//                    course->display();
//                }
//            }
//        }
//    }
//    
//    void searchByStudent(const string& studentName) {
//        cout << "Курсы студента " << studentName << ":\n";
//        for (const auto& curriculum : curricula) {
//            for (const auto& course : curriculum->getCourses()) {
//                for (const auto& student : course->getStudents()) {
//                    if (student->getName() == studentName) {
//                        cout << course->getTitle() << "\n";
//                    }
//                }
//            }
//        }
//        cout << "\n";
//    }
//};
//
//int main() {
//    Syllabus syllabus1("Основы программирования на C++");
//    Syllabus syllabus2("Алгоритмы и структуры данных");
//    Syllabus syllabus3("Базы данных");
//    
//    Student student1("Иван Иванов");
//    Student student2("Петр Петров");
//    Student student3("Мария Сидорова");
//    
//    Course course1("C++ для начинающих", "Изучение основ C++", 40, "Алексей Смирнов", &syllabus1);
//    Course course2("Алгоритмы", "Изучение алгоритмов", 60, "Ольга Ковалева", &syllabus2);
//    Course course3("Базы данных", "Основы SQL", 50, "Алексей Смирнов", &syllabus3);
//    
//    course1.addStudent(&student1);
//    course1.addStudent(&student2);
//    course2.addStudent(&student2);
//    course2.addStudent(&student3);
//    course3.addStudent(&student1);
//    course3.addStudent(&student3);
//    
//    Curriculum curriculum1;
//    curriculum1.addCourse(&course1);
//    curriculum1.addCourse(&course2);
//    
//    Curriculum curriculum2;
//    curriculum2.addCourse(&course3);
//    
//    EducationalPlatform platform;
//    platform.addCurriculum(&curriculum1);
//    platform.addCurriculum(&curriculum2);
//    
//    platform.displayAllCourses();
//    platform.searchByTeacher("Алексей Смирнов");
//    platform.searchByStudent("Иван Иванов");
//    
//    return 0;
//}
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MAX {
private:
    string name;
public:
    MAX(const string& n) : name(n) {}
    string getName() const { return name; }
};

class ExplanatoryNote {
private:
    string content;
    string date;
public:
    ExplanatoryNote(const string& c, const string& d) : content(c), date(d) {}
    string getContent() const { return content; }
    string getDate() const { return date; }
    void display() const {
        cout << "Объяснительная от " << date << ": " << content << "\n";
    }
};

class ZamDekan {
private:
    string name;
    MAX* messenger;
    vector<ExplanatoryNote*> explanatoryNotes;
public:
    ZamDekan(const string& n, MAX* m) : name(n), messenger(m) {}
    
    ~ZamDekan() {
        for (auto note : explanatoryNotes) {
            delete note;
        }
    }
    
    void addExplanatoryNote(const string& content, const string& date) {
        explanatoryNotes.push_back(new ExplanatoryNote(content, date));
    }
    
    void displayInfo() const {
        cout << "Зам декана: " << name << "\n";
        if (messenger) {
            cout << "Мессенджер: " << messenger->getName() << "\n";
        }
        cout << "Объяснительные:\n";
        for (const auto& note : explanatoryNotes) {
            note->display();
        }
        cout << "\n";
    }
    
    string getName() const { return name; }
};

int main() {
    MAX messenger("Макс");
    
    ZamDekan bedoidze("Бедоидзе М.В.", &messenger);
    
    bedoidze.addExplanatoryNote("Опоздал на пару из-за пробок", "2024-01-15");
    bedoidze.addExplanatoryNote("Не сдал вовремя лабораторную работу", "2024-01-20");
    bedoidze.addExplanatoryNote("Пропустил экзамен по болезни", "2024-02-01");
    
    bedoidze.displayInfo();
    
    return 0;
}
