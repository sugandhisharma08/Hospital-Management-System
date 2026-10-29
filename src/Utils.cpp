// Utils.cpp - implementation of the helper functions declared in Utils.h
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <filesystem>

namespace Utils {

std::string trim(const std::string& text) {
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) start++;
    std::size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) end--;
    return text.substr(start, end - start);
}

// The data files use | ; ~ as separators, so these characters are removed from user text.
std::string clean(const std::string& text) {
    std::string result;
    for (std::size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '|' || c == ';' || c == '~' || static_cast<unsigned char>(c) < 32) c = ' ';
        result += c;
    }
    return trim(result);
}

std::string toLower(const std::string& text) {
    std::string result = text;
    for (std::size_t i = 0; i < result.size(); i++)
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    return result;
}

bool containsIgnoreCase(const std::string& text, const std::string& part) {
    return toLower(text).find(toLower(part)) != std::string::npos;
}

std::vector<std::string> split(const std::string& text, char separator) {
    std::vector<std::string> parts;
    std::string current;
    for (std::size_t i = 0; i < text.size(); i++) {
        if (text[i] == separator) {
            parts.push_back(current);
            current.clear();
        } else {
            current += text[i];
        }
    }
    parts.push_back(current);
    return parts;
}

std::string fit(const std::string& text, std::size_t width) {
    if (text.size() < width) return text;
    if (width < 3) return text.substr(0, width);
    return text.substr(0, width - 2) + ".";
}

int toInt(const std::string& text, const std::string& fieldName) {
    std::string t = trim(text);
    if (t.empty() || t.size() > 9) throw HospitalException("Invalid number in field: " + fieldName);
    std::size_t i = (t[0] == '-') ? 1 : 0;
    if (i >= t.size()) throw HospitalException("Invalid number in field: " + fieldName);
    for (; i < t.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(t[i])))
            throw HospitalException("Invalid number in field: " + fieldName);
    }
    return std::atoi(t.c_str());
}

double toDouble(const std::string& text, const std::string& fieldName) {
    std::istringstream in(trim(text));
    double value = 0;
    std::string extra;
    if (!(in >> value) || (in >> extra) || value != value)
        throw HospitalException("Invalid amount in field: " + fieldName);
    return value;
}

std::string toFixed2(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

std::string formatMoney(double amount) {
    return "Rs. " + toFixed2(amount);
}

bool isValidPhone(const std::string& text) {
    if (text.size() != 10) return false;
    for (std::size_t i = 0; i < text.size(); i++)
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) return false;
    return true;
}

bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (std::size_t i = 0; i < date.size(); i++) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && leap) maxDay = 29;
    return day <= maxDay;
}

bool isValidTime(const std::string& time) {
    if (time.size() != 5 || time[2] != ':') return false;
    for (std::size_t i = 0; i < time.size(); i++) {
        if (i == 2) continue;
        if (!std::isdigit(static_cast<unsigned char>(time[i]))) return false;
    }
    int hour = std::atoi(time.substr(0, 2).c_str());
    int minute = std::atoi(time.substr(3, 2).c_str());
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

int timeToMinutes(const std::string& time) {
    return std::atoi(time.substr(0, 2).c_str()) * 60 + std::atoi(time.substr(3, 2).c_str());
}

std::string todayDate() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
    return std::string(buffer);
}

std::string nowTime() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%H:%M", local);
    return std::string(buffer);
}

void ensureParentDirectory(const std::string& filePath) {
    std::filesystem::path parent = std::filesystem::path(filePath).parent_path();
    if (parent.empty()) return;
    std::error_code ec;
    std::filesystem::create_directories(parent, ec);
}

// ------------------------------------------------------------------
// Keyboard input
// ------------------------------------------------------------------
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw InputEnded();
    return trim(line);
}

std::string readText(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::string text = clean(readLine(prompt));
        if (text.empty() && !allowEmpty) {
            std::cout << "  This field cannot be empty.\n";
            continue;
        }
        if (text.size() > 100) {
            std::cout << "  Too long. Please use at most 100 characters.\n";
            continue;
        }
        return text;
    }
}

int readInt(const std::string& prompt, int low, int high) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            int value = toInt(line, "number");
            if (value >= low && value <= high) return value;
            std::cout << "  Please enter a number between " << low << " and " << high << ".\n";
        } catch (const HospitalException&) {
            std::cout << "  Invalid input. Please enter a whole number.\n";
        }
    }
}

double readMoney(const std::string& prompt, double low, double high) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            double value = toDouble(line, "amount");
            if (value >= low && value <= high) return value;
            std::cout << "  Amount must be between " << toFixed2(low) << " and " << toFixed2(high) << ".\n";
        } catch (const HospitalException&) {
            std::cout << "  Invalid amount. Example: 500 or 499.50\n";
        }
    }
}

std::string readPhone(const std::string& prompt) {
    while (true) {
        std::string phone = readLine(prompt);
        if (isValidPhone(phone)) return phone;
        std::cout << "  Phone number must contain exactly 10 digits.\n";
    }
}

std::string readDate(const std::string& prompt, bool mustBeTodayOrLater) {
    while (true) {
        std::string date = readLine(prompt);
        if (!isValidDate(date)) {
            std::cout << "  Invalid date. Use the format YYYY-MM-DD (example 2026-12-25).\n";
            continue;
        }
        if (mustBeTodayOrLater && date < todayDate()) {
            std::cout << "  The date cannot be in the past (today is " << todayDate() << ").\n";
            continue;
        }
        return date;
    }
}

std::string readTime(const std::string& prompt) {
    while (true) {
        std::string time = readLine(prompt);
        if (isValidTime(time)) return time;
        std::cout << "  Invalid time. Use 24-hour HH:MM (example 14:30).\n";
    }
}

char readGender(const std::string& prompt) {
    while (true) {
        std::string text = toLower(readLine(prompt));
        if (text == "m" || text == "male") return 'M';
        if (text == "f" || text == "female") return 'F';
        if (text == "o" || text == "other") return 'O';
        std::cout << "  Please enter M, F or O.\n";
    }
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        std::string text = toLower(readLine(prompt));
        if (text == "y" || text == "yes") return true;
        if (text == "n" || text == "no") return false;
        std::cout << "  Please answer y or n.\n";
    }
}

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}

void printLine(char symbol, int length) {
    std::cout << std::string(length, symbol) << "\n";
}

void printTitle(const std::string& title) {
    std::cout << "\n";
    printLine('=');
    std::cout << "  " << title << "\n";
    printLine('=');
}

} // namespace Utils
