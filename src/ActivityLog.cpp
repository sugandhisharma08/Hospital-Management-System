// ActivityLog.cpp
#include "ActivityLog.h"
#include "Utils.h"

ActivityLog::ActivityLog() : path("") {}

ActivityLog::~ActivityLog() {
    if (file.is_open()) {
        write("Program closed");
        file.close();
    }
}

void ActivityLog::open(const std::string& logPath) {
    path = logPath;
    Utils::ensureParentDirectory(path);
    file.open(path.c_str(), std::ios::app);      // append mode: old lines are kept
}

void ActivityLog::write(const std::string& message) {
    if (!file.is_open()) return;                 // logging is optional: ignore if the file cannot be opened
    file << Utils::todayDate() << " " << Utils::nowTime() << " | " << message << "\n";
    file.flush();
}

std::vector<std::string> ActivityLog::readLastLines(int count) const {
    std::vector<std::string> lines;
    std::ifstream in(path.c_str());
    std::string line;
    while (std::getline(in, line)) lines.push_back(line);
    if (static_cast<int>(lines.size()) > count)
        lines.erase(lines.begin(), lines.end() - count);
    return lines;
}
