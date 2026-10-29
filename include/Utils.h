// Utils.h - helper functions used by every module (input checking, text and date helpers)
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <stdexcept>

// Custom exception class. Every validation / business-rule error in the project
// is reported by throwing a HospitalException (inheritance from std::runtime_error).
class HospitalException : public std::runtime_error {
public:
    explicit HospitalException(const std::string& message) : std::runtime_error(message) {}
};

// Thrown when keyboard input ends (Ctrl+D or the end of a piped test file).
class InputEnded : public std::runtime_error {
public:
    InputEnded() : std::runtime_error("Input ended") {}
};

namespace Utils {
    // ---- text helpers ----
    std::string trim(const std::string& text);
    std::string clean(const std::string& text);      // removes | ; ~ and control characters
    std::string toLower(const std::string& text);
    bool containsIgnoreCase(const std::string& text, const std::string& part);
    std::vector<std::string> split(const std::string& text, char separator);
    std::string fit(const std::string& text, std::size_t width);   // shortens text for table columns

    // ---- conversion helpers (throw HospitalException on bad data) ----
    int toInt(const std::string& text, const std::string& fieldName);
    double toDouble(const std::string& text, const std::string& fieldName);
    std::string toFixed2(double value);
    std::string formatMoney(double amount);

    // ---- validation helpers ----
    bool isValidPhone(const std::string& text);
    bool isValidDate(const std::string& date);       // YYYY-MM-DD
    bool isValidTime(const std::string& time);       // HH:MM (24 hour)
    int timeToMinutes(const std::string& time);
    std::string todayDate();
    std::string nowTime();

    // ---- file helper ----
    void ensureParentDirectory(const std::string& filePath);

    // ---- keyboard input helpers (they keep asking until the input is valid) ----
    std::string readLine(const std::string& prompt);
    std::string readText(const std::string& prompt, bool allowEmpty = false);
    int readInt(const std::string& prompt, int low, int high);
    double readMoney(const std::string& prompt, double low, double high);
    std::string readPhone(const std::string& prompt);
    std::string readDate(const std::string& prompt, bool mustBeTodayOrLater);
    std::string readTime(const std::string& prompt);
    char readGender(const std::string& prompt);
    bool readYesNo(const std::string& prompt);

    // ---- screen helpers ----
    void pause();
    void printLine(char symbol = '-', int length = 72);
    void printTitle(const std::string& title);
}

#endif
