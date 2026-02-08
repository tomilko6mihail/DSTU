#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class MovieSession {
private:
    std::string movieTitle;
    std::string sessionTime;
    std::string sessionDate;
    double averageTicketPrice;
    int soldTickets;
    int totalSeats;

public:
    MovieSession() : movieTitle(""), sessionTime(""), sessionDate(""),
                    averageTicketPrice(0.0), soldTickets(0), totalSeats(0) {}
    
    MovieSession(const std::string& title, const std::string& time, const std::string& date,
                double price, int tickets, int seats = 200)
        : movieTitle(title), sessionTime(time), sessionDate(date),
          averageTicketPrice(price), soldTickets(tickets), totalSeats(seats) {}
    
    MovieSession(const MovieSession& other)
        : movieTitle(other.movieTitle), sessionTime(other.sessionTime),
          sessionDate(other.sessionDate), averageTicketPrice(other.averageTicketPrice),
          soldTickets(other.soldTickets), totalSeats(other.totalSeats) {}
        std::string getMovieTitle() const { return movieTitle; }
    std::string getSessionTime() const { return sessionTime; }
    std::string getSessionDate() const { return sessionDate; }
    double getAverageTicketPrice() const { return averageTicketPrice; }
    int getSoldTickets() const { return soldTickets; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return totalSeats - soldTickets; }  // Свободные места
    
    void setMovieTitle(const std::string& title) { movieTitle = title; }
    void setSessionTime(const std::string& time) { sessionTime = time; }
    void setSessionDate(const std::string& date) { sessionDate = date; }
    void setAverageTicketPrice(double price) { averageTicketPrice = price; }
    void setSoldTickets(int tickets) {
        if (tickets >= 0 && tickets <= totalSeats) {
            soldTickets = tickets;
        }
    }
    void setTotalSeats(int seats) {
        if (seats >= soldTickets) {
            totalSeats = seats;
        }
    }
    
    double calculateRevenue() const {
        return averageTicketPrice * soldTickets;
    }
    
    bool isDateInRange(const std::string& startDate, const std::string& endDate) const {
        return sessionDate >= startDate && sessionDate <= endDate;
    }
    
    bool sellTicket() {
        if (soldTickets < totalSeats) {
            soldTickets++;
            return true;
        }
        return false;
    }
    
    // Возврат билета (отказ от билета)
    bool returnTicket() {
        if (soldTickets > 0) {
            soldTickets--;
            return true;
        }
        return false;
    }
    
    MovieSession& operator=(const MovieSession& other) {
        if (this != &other) {
            movieTitle = other.movieTitle;
            sessionTime = other.sessionTime;
            sessionDate = other.sessionDate;
            averageTicketPrice = other.averageTicketPrice;
            soldTickets = other.soldTickets;
            totalSeats = other.totalSeats;
        }
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const MovieSession& session) {
        os << "Фильм: " << session.movieTitle
           << " | Время: " << session.sessionTime
           << " | Дата: " << session.sessionDate
           << " | Цена: " << session.averageTicketPrice
           << " | Продано: " << session.soldTickets << "/" << session.totalSeats
           << " | Свободно: " << session.getAvailableSeats()
           << " | Выручка: " << session.calculateRevenue();
        return os;
    }
    
    bool operator==(const MovieSession& other) const {
        return movieTitle == other.movieTitle &&
               sessionTime == other.sessionTime &&
               sessionDate == other.sessionDate;
    }
    
    bool operator!=(const MovieSession& other) const {
        return !(*this == other);
    }
    
    bool operator<(const MovieSession& other) const {
        if (sessionDate != other.sessionDate)
            return sessionDate < other.sessionDate;
        return sessionTime < other.sessionTime;
    }
    
    bool operator>(const MovieSession& other) const {
        return other < *this;
    }
    
    MovieSession operator+(double priceIncrease) const {
        MovieSession result = *this;
        result.averageTicketPrice += priceIncrease;
        return result;
    }
    
    MovieSession operator-(double priceDecrease) const {
        return *this + (-priceDecrease);
    }
    
    MovieSession operator*(double multiplier) const {
        MovieSession result = *this;
        result.averageTicketPrice *= multiplier;
        return result;
    }
    
    MovieSession operator+(const MovieSession& other) const {
        MovieSession result = *this;
        if (movieTitle == other.movieTitle && sessionDate == other.sessionDate) {
            result.soldTickets += other.soldTickets;
            result.averageTicketPrice = (averageTicketPrice + other.averageTicketPrice) / 2.0;
        }
        return result;
    }
    

    MovieSession& operator--() {
        if (soldTickets > 0) {
            soldTickets--;
            std::cout << " * Один билет возвращен в продажу для фильма '"
                      << movieTitle << "'\n";
        } else {
            std::cout << " * Нет проданных билетов для возврата для фильма '"
                      << movieTitle << "'\n";
        }
        return *this;
    }
    
    MovieSession operator--(int) {
        MovieSession temp = *this;
        --(*this);
        return temp;
    }
    
    MovieSession& operator++() {
        if (soldTickets < totalSeats) {
            soldTickets++;
            std::cout << " * Один билет продан для фильма '" << movieTitle << "'\n";
        } else {
            std::cout << " * Нет свободных мест для фильма '" << movieTitle << "'\n";
        }
        return *this;
    }
    
    MovieSession operator++(int) {
        MovieSession temp = *this;
        ++(*this);
        return temp;
    }
};

class Cinema {
private:
    std::vector<MovieSession> sessions;

public:
    // Конструкторы
    Cinema() = default;
    Cinema(const std::vector<MovieSession>& initialSessions) : sessions(initialSessions) {}
    
    // Методы
    void addSession(const MovieSession& session) {
        sessions.push_back(session);
    }
    
    double calculateTotalRevenue() const {
        double total = 0.0;
        for (const auto& session : sessions) {
            total += session.calculateRevenue();
        }
        return total;
    }
    
    std::vector<std::string> getMoviesInPeriod(const std::string& startDate, const std::string& endDate) const {
        std::vector<std::string> movies;
        for (const auto& session : sessions) {
            if (session.isDateInRange(startDate, endDate)) {
                if (std::find(movies.begin(), movies.end(), session.getMovieTitle()) == movies.end()) {
                    movies.push_back(session.getMovieTitle());
                }
            }
        }
        return movies;
    }
    
    void displayAllSessions() const {
        std::cout << "Все сеансы кинотеатра:\n";
        for (size_t i = 0; i < sessions.size(); ++i) {
            std::cout << i + 1 << ". " << sessions[i] << std::endl;
        }
    }
    

    MovieSession* findSession(const std::string& movieTitle) {
        for (auto& session : sessions) {
            if (session.getMovieTitle() == movieTitle) {
                return &session;
            }
        }
        return nullptr;
    }
    

    MovieSession& operator[](size_t index) {
        if (index >= sessions.size()) {
            throw std::out_of_range("Индекс выходит за границы");
        }
        return sessions[index];
    }
    
    const MovieSession& operator[](size_t index) const {
        if (index >= sessions.size()) {
            throw std::out_of_range("Индекс выходит за границы");
        }
        return sessions[index];
    }
    

    size_t size() const {
        return sessions.size();
    }
};


int main() {
    MovieSession session1("Аватар", "14:00", "2024-01-15", 350.0, 120, 150);
    MovieSession session2("Аватар", "18:00", "2024-01-15", 400.0, 150, 180);
    MovieSession session3("Интерстеллар", "16:30", "2024-01-16", 380.0, 90, 120);
    MovieSession session4("Начало", "20:00", "2024-01-17", 420.0, 110, 130);
    
    Cinema cinema;
    cinema.addSession(session1);
    cinema.addSession(session2);
    cinema.addSession(session3);
    cinema.addSession(session4);
    
    std::cout << "=== ТЕСТИРОВАНИЕ ПРОГРАММЫ УЧЕТА КИНОТЕАТРА ===\n\n";
    
    cinema.displayAllSessions();
    
    std::cout << "\nОбщая выручка кинотеатра: " << cinema.calculateTotalRevenue() << " руб.\n";
    
    std::cout << "\n=== ДЕМОНСТРАЦИЯ ПРОДАЖИ И ВОЗВРАТА БИЛЕТОВ ===\n";
    
    std::cout << "\n1. Продажа билетов (оператор ++):\n";
    ++cinema[0];
    cinema[1]++;

    std::cout << "\n2. Возврат билетов (оператор --):\n";
    --cinema[0];
    cinema[2]--;
    
    std::cout << "\n3. Обновленное состояние сеансов:\n";
    cinema.displayAllSessions();
    
    std::cout << "\n=== СИМУЛЯЦИЯ ПРОЦЕССА ПОКУПКИ И ВОЗВРАТА ===\n";
    
    MovieSession testSession("Матрица", "19:00", "2024-01-18", 300.0, 5, 10);
    std::cout << "\nНачальное состояние:\n" << testSession << std::endl;
    
    std::cout << "\nПокупаем 3 билета:\n";
    ++testSession;
    testSession++;
    ++testSession;
    
    std::cout << "\nПосле покупки:\n" << testSession << std::endl;
    
    std::cout << "\nВозвращаем 2 билета:\n";
    --testSession;
    testSession--;
    
    std::cout << "\nПосле возврата:\n" << testSession << std::endl;
    
    std::cout << "\nПопытка вернуть больше билетов, чем продано:\n";
    for (int i = 0; i < 10; i++) {
        testSession--;
    }
    
    std::cout << "\nФинальное состояние:\n" << testSession << std::endl;
    
    std::cout << "\n=== ДРУГИЕ ПЕРЕГРУЖЕННЫЕ ОПЕРАЦИИ ===\n";
    
    MovieSession expensiveSession = cinema[0] + 100.0;
    std::cout << "\nПовышение цены на 100 руб:\n" << expensiveSession << std::endl;
    
    std::cout << "\nСравнение сеансов:\n";
    std::cout << "session1 == session2: " << (cinema[0] == cinema[1] ? "true" : "false") << std::endl;
    std::cout << "session1 < session3: " << (cinema[0] < cinema[2] ? "true" : "false") << std::endl;
    
    return 0;
}
