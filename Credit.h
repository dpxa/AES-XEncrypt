#include "main.h"

class Date {
    private:
    int month, day = 0;

    public:
    Date() {}
    Date(int m, int d) { month = m; day = d; }
    
    void set_all(int m, int d) { month = m; day = d; }
    
    int return_month() { return month; }
    int return_day() { return day; } 
};

class Credit {
    private:
    std::string num;
    int ccv = 0;
    Date exp_date;

    public:
    Credit() {}
    Credit(std::string n, int c, Date ed) { num = n; ccv = c; exp_date = ed; }
    
    void set_all(std::string n, int c, Date ed) { num = n; ccv = c; exp_date = ed; }
    
    void save(std::string file) {
        std::ofstream fout(file);
        fout << "Credit card number: " << num << "\nCCV: " << ccv;
        fout << "\nDate: " << exp_date.return_month() << '/' << exp_date.return_day() << "\n\n";
    }
};