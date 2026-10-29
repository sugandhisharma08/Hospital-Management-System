// DataManager.h - abstract base class that knows how to load and save a text data file.
// Every module manager (patients, doctors, ...) inherits from it and only has to say
// how ONE line is read (parseLine) and how all records are written (writeRecords).
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <ostream>

class DataManager {
protected:
    std::string filePath;
    int skippedLines;            // lines in the file that were invalid or duplicated

    virtual bool parseLine(const std::string& line) = 0;          // true if the record was stored
    virtual void writeRecords(std::ostream& out) const = 0;

public:
    explicit DataManager(const std::string& path);
    virtual ~DataManager();

    int load();                  // returns number of records loaded (a missing file is fine)
    bool save() const;           // returns false if the file could not be written
    int getSkippedLines() const;
    std::string getFilePath() const;
    virtual std::string moduleName() const = 0;
};

#endif
