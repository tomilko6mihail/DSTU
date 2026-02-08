#include "finance_tracker.h"

bool FinanceTracker::validationFloat(const string str) {
    bool flagDot = false;
    if (str.empty()) return false;
    int startIndex = str[0] == '-' ? 1 : 0;
    for (int i = startIndex; i < str.length(); i++) {
        if (str[0] == '.') return false;
        if (isspace(str[i])) return false;
        if (str[i] == '.') {
            if (flagDot) return false;
            flagDot = true;
        } else if (!isdigit(str[i])) return false;
    }
    return startIndex < str.length();
}

bool FinanceTracker::validationInt(const string str) {
    if (str.empty()) return false;
    int startIndex = str[0] == '-' ? 1 : 0;
    for (int i = startIndex; i < str.length(); i++) {
        if (str[0] == '.' || str[i] == '.') return false;
        if (isspace(str[i])) return false;
        if (!isdigit(str[i])) return false;
    }
    return startIndex < str.length();
}

bool FinanceTracker::validationDate(const string str) {
    if (str.length() != 10 || str[4] != '-' || str[7] != '-') {
        cout << "Неверный формат даты!" << endl;
        return false;
    }
    string year = str.substr(0, 4), month = str.substr(5, 2), day = str.substr(8, 2);
    if (!validationInt(year) || !validationInt(month) || !validationInt(day)) {
        cout << "Дата содержит некорректные символы!" << endl;
        return false;
    }
    int m = stoi(month), d = stoi(day);
    if (m < 1 || m > 12 || d < 1 || d > 31) {
        cout << "Недопустимые значения месяца или дня!" << endl;
        return false;
    }
    return true;
}

string FinanceTracker::normalizeDate(const string& date) {
    string year = date.substr(0, 4);
    string month = date.substr(5, 2);
    string day = date.substr(8, 2);
    try {
        if (month.length() == 1) month = "0" + month;
        if (day.length() == 1) day = "0" + day;
    } catch (const out_of_range& e) {
        return date;
    }
    return year + "-" + month + "-" + day;
}

void FinanceTracker::addTransaction() {
    string date, amountStr, category, description;
    float amount;
    int id = rand() % 10000 + 1;
    
    cout << "Дата (YYYY-MM-DD): ";
    getline(cin, date);
    if (!validationDate(date)) {
        cout << "Неверная дата!" << endl;
        return;
    }
    date = normalizeDate(date);
    
    cout << "Сумма (положительная для дохода, отрицательная для расхода): ";
    getline(cin, amountStr);
    if (validationFloat(amountStr)) {
        amount = stof(amountStr);
    } else {
        cout << "Неверная сумма!" << endl;
        return;
    }
    
    cout << "Категория: ";
    getline(cin, category);
    for (char& ch : category) ch = tolower(ch);
    
    cout << "Описание: ";
    getline(cin, description);
    
    transactions.emplace_back(date, amount, category, description, id);
    cout << "Транзакция добавлена с ID: " << id << endl;
    
    cout << "Текущие транзакции:" << endl;
    for (const auto& t : transactions) {
        cout << t.date << ", " << t.amount << ", " << t.description << ", ID: " << t.id << endl;
    }
}

void FinanceTracker::generateDailyReport() {
    string date;
    cout << "Дата для отчета (YYYY-MM-DD): ";
    getline(cin, date);
    if (!validationDate(date)) return;
    date = normalizeDate(date);
    
    float dailySum = getDailySum(date);
    int dailyCount = getDailyCount(date);
    
    vector<pair<string, float>> descs;
    for (const auto& t : transactions){
        if (t.date == date){
            descs.push_back({t.description, t.amount});
        }
    }
    
    float prevSum = getPreviousDaySum(date);
    string monthYear = date.substr(5, 2) + "." + date.substr(0, 4);
    float monthlySum = getMonthlySum(monthYear);
    
    string filename = "report_" + date + ".txt";
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка создания файла!" << endl;
        return;
    }
    
    string dayWeek = getDayOfWeek(date);
    file << date.substr(8,2) + "." + date.substr(5,2) + "." + date.substr(0,4) << " " << dayWeek << endl;
    file << dailyCount << " транзакции на общую сумму " << dailySum << " руб." << endl;
    for (size_t i = 0; i < descs.size(); ++i) {
        string type = descs[i].second < 0 ? "трата" : "зачисление";
        file << type << ": " << (descs[i].second < 0 ? "" : "+") << descs[i].second << "руб (" << descs[i].first << ")" << endl;
    }
    if (prevSum != 0) {
        float diff = dailySum - prevSum;
        file << "в этот день " << (diff > 0 ? "получено" : "потрачено") << " на " << abs(diff) << " рублей "
             << (diff > 0 ? "больше" : "меньше") << ", чем вчера." << endl;
    }
    file << "за " << date.substr(5,2) + "." + date.substr(0,4) << (monthlySum < 0 ? " вы в минусе на" : " вы в плюсе на") << ": " << abs(monthlySum) << " рублей." << endl;
    
    file.close();
    cout << "Отчет сгенерирован в " << filename << endl;
}

string FinanceTracker::getDayOfWeek(const string& date) {
    tm time_in = {0};
    time_in.tm_year = stoi(date.substr(0,4)) - 1900;
    time_in.tm_mon = stoi(date.substr(5,2)) - 1;
    time_in.tm_mday = stoi(date.substr(8,2));
    mktime(&time_in);
    const string days[] = {"воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"};
    return days[time_in.tm_wday];
}

float FinanceTracker::getDailySum(const string& date) {
    float sum = 0;
    for (const auto& t : transactions) if (t.date == date) sum += t.amount;
    return sum;
}

int FinanceTracker::getDailyCount(const string& date) {
    int count = 0;
    for (const auto& t : transactions) if (t.date == date) count++;
    return count;
}

float FinanceTracker::getPreviousDaySum(const string& date) {
    tm time_in = {0};
    time_in.tm_year = stoi(date.substr(0,4)) - 1900;
    time_in.tm_mon = stoi(date.substr(5,2)) - 1;
    time_in.tm_mday = stoi(date.substr(8,2)) - 1;
    mktime(&time_in);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", &time_in);
    string prevDate = buffer;
    return getDailySum(prevDate);
}

float FinanceTracker::getMonthlySum(const string& monthYear) {
    float sum = 0;
    string month = monthYear.substr(0,2), year = monthYear.substr(3,4);
    for (const auto& t : transactions) {
        if (t.date.substr(5,2) == month && t.date.substr(0,4) == year) sum += t.amount; // Убрано amount < 0
    }
    return sum;
}

bool FinanceTracker::equal(int N, int S) {
    if (N == 0) return S == 0;
    return equal(N / 10, S - (N % 10));
}

void FinanceTracker::checkTransactionID() {
    string idStr, sumStr;
    int id, sum;
    cout << "ID транзакции: ";
    getline(cin, idStr);
    if (validationInt(idStr)) id = stoi(idStr); else return;
    cout << "Ожидаемая сумма цифр: ";
    getline(cin, sumStr);
    if (validationInt(sumStr)) sum = stoi(sumStr); else return;
    cout << "Проверка: " << (equal(id, sum) ? "Верно" : "Неверно") << endl;
}

void FinanceTracker::generateRandomData() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1000.0, 1000.0);
    uniform_int_distribution<> idDis(1, 10000);
    
    int count;
    string countStr, date = "2025-09-16";
    cout << "Количество случайных транзакций: ";
    getline(cin, countStr);
    if (validationInt(countStr)) count = stoi(countStr); else return;
    
    for (int i = 0; i < count; ++i) {
        float amount = dis(gen);
        string category = "random";
        string desc = "test " + to_string(i);
        int id = idDis(gen);
        transactions.emplace_back(date, amount, category, desc, id);
    }
    cout << "Сгенерировано " << count << " транзакций." << endl;
}
