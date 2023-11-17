#include <iostream>
#include <string>
// AD - наше время
//BC - до нашей эры

class Date {
public:
    Date(int year = 1960, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0, bool isOurEra = true)
        : year_(year), month_(month), day_(day), hour_(hour), minute_(minute), second_(second) {
        // Эра (AD или BC) в зависимости от знака года
        if (year_ < 0) {
            isOurEra_ = false;
            year_ = -year_;
        }
        else {
            isOurEra_ = isOurEra;
        }
    }

    Date(const Date& other)
        : year_(other.year_), month_(other.month_), day_(other.day_), hour_(other.hour_), minute_(other.minute_),
        second_(other.second_), isOurEra_(other.isOurEra_) {}

    Date& operator=(const Date& other) {
        if (this != &other) {
            year_ = other.year_;
            month_ = other.month_;
            day_ = other.day_;
            hour_ = other.hour_;
            minute_ = other.minute_;
            second_ = other.second_;
            isOurEra_ = other.isOurEra_;
        }

        return *this;
    }

    Date operator+(const Date& other) const {
        Date result = *this;
        return result += other;
    }

    Date& operator+=(const Date& other) {
        second_ += other.second_;
        minute_ += other.minute_;
        hour_ += other.hour_;
        day_ += other.day_;
        month_ += other.month_;
        year_ += other.year_;

        normalize();

        return *this;
    }

    Date operator-(const Date& other) const {
        Date result = *this;
        return result -= other;
    }

    Date& operator-=(const Date& other) {
        second_ -= other.second_;
        minute_ -= other.minute_;
        hour_ -= other.hour_;
        day_ -= other.day_;
        month_ -= other.month_;
        year_ -= other.year_;

        if (year_ < 0) {
            isOurEra_ = false;
            year_ = -year_;
        }

        normalize();
        
        return *this;
    }

    bool operator<(const Date& other) const {
        return compare(other) < 0;
    }

    bool operator>(const Date& other) const {
        return compare(other) > 0;
    }

    bool operator==(const Date& other) const {
        return compare(other) == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.toString();
        return os;
    }

    Date add(int years, int months, int days, int hours, int minutes, int seconds) const {
        Date result = *this;
        result.year_ += years;
        result.month_ += months;
        result.day_ += days;
        result.hour_ += hours;
        result.minute_ += minutes;
        result.second_ += seconds;

        result.normalize();
        return result;
    }

    Date subtract(int years, int months, int days, int hours, int minutes, int seconds) const {
        return add(-years, -months, -days, -hours, -minutes, -seconds);
    }

private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;
    bool isOurEra_;

    static int daysInMonth_[13];

    void normalize() {
        if (second_ >= 60) {
            minute_ += second_ / 60;
            second_ %= 60;
        }
        else if (second_ < 0) {
            int minutes = (-second_ - 1) / 60 + 1;
            minute_ -= minutes;
            second_ += minutes * 60;
        }

        if (minute_ >= 60) {
            hour_ += minute_ / 60;
            minute_ %= 60;
        }
        else if (minute_ < 0) {
            int hours = (-minute_ - 1) / 60 + 1;
            hour_ -= hours;
            minute_ += hours * 60;
        }

        if (hour_ >= 24) {
            day_ += hour_ / 24;
            hour_ %= 24;
        }
        else if (hour_ < 0) {
            int days = (-hour_ - 1) / 24 + 1;
            day_ -= days;
            hour_ += days * 24;
        }

        while (month_ > 12) {
            year_++;
            month_ -= 12;
        }
        while (month_ < 1) {
            year_--;
            month_ += 12;
        }

        if (year_ < 0) {
            isOurEra_ = false;
            year_ = -year_;
        }

        if (isLeapYear(year_)) {
            daysInMonth_[2] = 29;
        }
        else {
            daysInMonth_[2] = 28;
        }

        while (day_ > daysInMonth_[month_]) {
            day_ -= daysInMonth_[month_];
            month_++;
            if (month_ > 12) {
                year_++;
                month_ = 1;
            }
        }
        while (day_ < 1) {
            month_--;
            if (month_ < 1) {
                year_--;
                month_ = 12;
            }
            day_ += daysInMonth_[month_];
        }
    }

    int compare(const Date& other) const {
        if (year_ != other.year_) {
            return year_ - other.year_;
        }
        if (month_ != other.month_) {
            return month_ - other.month_;
        }
        if (day_ != other.day_) {
            return day_ - other.day_;
        }
        if (hour_ != other.hour_) {
            return hour_ - other.hour_;
        }
        if (minute_ != other.minute_) {
            return minute_ - other.minute_;
        }
        if (second_ != other.second_) {
            return second_ - other.second_;
        }
        if (isOurEra_ != other.isOurEra_) {
            return isOurEra_ ? 1 : -1;
        }
        return 0;
    }

    bool isLeapYear(int year) const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    std::string toString() const {
        std::string era = isOurEra_ ? "AD" : "BC";
        return std::to_string(year_) + "-" + padZero(month_) + "-" + padZero(day_) + " " +
            padZero(hour_) + ":" + padZero(minute_) + ":" + padZero(second_) + " " + era;
    }

    std::string padZero(int num) const {
        if (num < 10) {
            return "0" + std::to_string(num);
        }
        else {
            return std::to_string(num);
        }
    }
};

int Date::daysInMonth_[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int main() {
    Date date1;  // Инициализация даты по умолчанию
    Date date2(1014, 11, 14, 15, 30, 0, true); // Здесь можно изменить значения

    std::cout << "Date 1: " << date1 << std::endl;
    std::cout << "Date 2: " << date2 << std::endl;

    Date date3 = date1 + date2;
    std::cout << "Date 3 (Date 1 + Date 2): " << date3 << std::endl;

    Date date4 = date2 - date1;
    std::cout << "Date 4 (Date 2 - Date 1): " << date4 << std::endl;

    return 0;
}
