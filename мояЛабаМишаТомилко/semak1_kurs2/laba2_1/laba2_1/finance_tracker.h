#ifndef FINANCE_TRACKER_H
#define FINANCE_TRACKER_H

#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <random>
#include <iostream>

using namespace std;

class FinanceTracker {
public:
    struct Transaction {
        string date;
        float amount;
        string category;
        string description;
        int id;
        
        Transaction(string d, float a, string c, string desc, int i) : date(d), amount(a), category(c), description(desc), id(i) {}
    };
    
    vector<Transaction> transactions;
    
    bool validationFloat(const string str);
    bool validationInt(const string str);
    bool validationDate(const string str);
    string normalizeDate(const string& date);
    
    void addTransaction();
    void generateDailyReport();
    void checkTransactionID();
    void generateRandomData();
    
    bool equal(int N, int S);
    
    string getDayOfWeek(const string& date);
    float getDailySum(const string& date);
    int getDailyCount(const string& date);
    float getPreviousDaySum(const string& date);
    float getMonthlySum(const string& monthYear);
};

#endif
