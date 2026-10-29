// StaffManager.cpp
#include "StaffManager.h"
#include "Utils.h"

StaffManager::StaffManager(const std::string& path) : DataManager(path) {}

bool StaffManager::parseLine(const std::string& line) {
    Staff s = Staff::fromRecord(line);
    if (exists(s.getId())) return false;
    staffList.push_back(s);
    Staff::syncNextId(s.getId());
    return true;
}

void StaffManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < staffList.size(); i++) out << staffList[i].toRecord() << "\n";
}

int StaffManager::count() const { return static_cast<int>(staffList.size()); }

bool StaffManager::exists(int id) const { return findById(id) != nullptr; }

Staff* StaffManager::findById(int id) {
    for (std::size_t i = 0; i < staffList.size(); i++)
        if (staffList[i].getId() == id) return &staffList[i];
    return nullptr;
}

const Staff* StaffManager::findById(int id) const {
    for (std::size_t i = 0; i < staffList.size(); i++)
        if (staffList[i].getId() == id) return &staffList[i];
    return nullptr;
}

std::vector<const Staff*> StaffManager::searchByText(const std::string& text) const {
    std::vector<const Staff*> result;
    for (std::size_t i = 0; i < staffList.size(); i++) {
        if (Utils::containsIgnoreCase(staffList[i].getName(), text) ||
            Utils::containsIgnoreCase(staffList[i].getRole(), text) ||
            Utils::containsIgnoreCase(staffList[i].getDepartment(), text))
            result.push_back(&staffList[i]);
    }
    return result;
}

const std::vector<Staff>& StaffManager::getAll() const { return staffList; }

void StaffManager::addStaff(const Staff& staff) {
    if (exists(staff.getId())) throw HospitalException("Staff ID already exists.");
    staffList.push_back(staff);
    Staff::syncNextId(staff.getId());
}

void StaffManager::removeStaff(int id) {
    for (std::size_t i = 0; i < staffList.size(); i++) {
        if (staffList[i].getId() == id) {
            staffList.erase(staffList.begin() + static_cast<std::ptrdiff_t>(i));
            return;
        }
    }
    throw HospitalException("Staff member not found.");
}

std::string StaffManager::moduleName() const { return "Staff"; }
