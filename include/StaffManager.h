// StaffManager.h - stores all staff members and saves/loads staff.txt
#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include <vector>
#include "DataManager.h"
#include "Staff.h"

class StaffManager : public DataManager {
private:
    std::vector<Staff> staffList;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit StaffManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    Staff* findById(int id);
    const Staff* findById(int id) const;
    std::vector<const Staff*> searchByText(const std::string& text) const;    // name, role or department
    const std::vector<Staff>& getAll() const;
    void addStaff(const Staff& staff);
    void removeStaff(int id);
    std::string moduleName() const override;
};

#endif
