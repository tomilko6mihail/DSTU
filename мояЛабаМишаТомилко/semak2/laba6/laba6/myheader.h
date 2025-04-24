#ifndef MYHEADER_H
#define MYHEADER_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;
class students {
public:
    bool validationInt(const std::string str);
    void addStudents();
    
    struct student {
        string fio;
        string subj;
        int count_peresdachi;
        
        student(string full_fio, string subject, int peresdachi) : fio(std::move(full_fio)), subj(std::move(subject)), count_peresdachi(std::move(peresdachi)) {}
    };
    
    vector<student> studenti;
    
    static bool sortirovka(const student& a, const student& b);
    void displayStudents();
    
    map<char, int> mapa;
    void mapBukvi();
    
};

#endif 
