#ifndef CREDIT_H
#define CREDIT_H

#include "main.h"

class Date {
private:
    int month;
    int day;

public:
    Date() : month(0), day(0) {}
    Date(int m, int d) : month(m), day(d) {}

    void setAll(int m, int d) {
        month = m;
        day = d;
    }

    int getMonth() const { return month; }
    int getDay() const { return day; }
};

class Credit {
private:
    std::string number;
    int ccv;
    Date expirationDate;

public:
    Credit() : number(""), ccv(0), expirationDate() {}
    Credit(const std::string& n, int c, const Date& ed) 
        : number(n), ccv(c), expirationDate(ed) {}

    void setAll(const std::string& n, int c, const Date& ed) {
        number = n;
        ccv = c;
        expirationDate = ed;
    }

    void save(const std::string& file) const {
        std::ofstream fout(file, std::ios::app);
        if (!fout)
            throw std::runtime_error("Failed to open file for writing credit information.");
        
        fout << "Credit card number: " << number << "\nCCV: " << ccv;
        fout << "\nDate: " << expirationDate.getMonth() << '/' << expirationDate.getDay() << "\n\n";
    }
};

#endif // CREDIT_H
