// DataManager.cpp - common file loading and saving logic
#include "DataManager.h"
#include "Utils.h"
#include <fstream>
#include <filesystem>
#include <exception>

DataManager::DataManager(const std::string& path) : filePath(path), skippedLines(0) {}

DataManager::~DataManager() {}

int DataManager::getSkippedLines() const { return skippedLines; }
std::string DataManager::getFilePath() const { return filePath; }

int DataManager::load() {
    skippedLines = 0;
    std::ifstream in(filePath.c_str());
    if (!in.is_open()) return 0;          // file does not exist yet: it is created on the first save
    int loaded = 0;
    std::string line;
    while (std::getline(in, line)) {
        line = Utils::trim(line);
        if (line.empty() || line[0] == '#') continue;      // skip blank lines and comments
        try {
            if (parseLine(line)) loaded++;
            else skippedLines++;
        } catch (const std::exception&) {
            skippedLines++;                                // invalid line: skip it safely
        }
    }
    return loaded;
}

// Data is written to a temporary file first and then renamed, so a crash in the middle
// of saving cannot destroy the old file.
bool DataManager::save() const {
    Utils::ensureParentDirectory(filePath);
    std::string tempPath = filePath + ".tmp";
    {
        std::ofstream out(tempPath.c_str());
        if (!out.is_open()) return false;
        out << "# " << moduleName() << " data file - one record per line, fields separated by |\n";
        writeRecords(out);
        out.flush();
        if (!out.good()) return false;
    }
    std::error_code ec;
    std::filesystem::rename(tempPath, filePath, ec);
    return !ec;
}
