// ActivityLog.h - writes a simple audit trail (activity_log.txt)
#ifndef ACTIVITYLOG_H
#define ACTIVITYLOG_H

#include <fstream>
#include <string>
#include <vector>

class ActivityLog {
private:
    std::ofstream file;          // the log file stays open while the program runs
    std::string path;

public:
    ActivityLog();
    ~ActivityLog();              // DESTRUCTOR: writes a closing line and closes the file

    void open(const std::string& logPath);
    void write(const std::string& message);
    std::vector<std::string> readLastLines(int count) const;
};

#endif
