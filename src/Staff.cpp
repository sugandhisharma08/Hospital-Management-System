// Staff.cpp - implementation of the Staff class
#include "Staff.h"
#include "Utils.h"
#include <iostream>

int Staff::nextId = 6001;

Staff::Staff(int sid, const std::string& sname, int sage, char sgender,
             const std::string& scontact, const std::string& saddress,
             const std::string& srole, const std::string& sdepartment, double ssalary)
    : Person(sid, sname, sage, sgender, scontact, saddress),
      role(""), department(""), salary(0) {
    setRole(srole);
    setDepartment(sdepartment);
    setSalary(ssalary);
}

Staff::~Staff() {}

std::string Staff::getRole() const { return role; }
std::string Staff::getDepartment() const { return department; }
double Staff::getSalary() const { return salary; }

void Staff::setRole(const std::string& value) {
    std::string text = Utils::clean(value);
    if (text.empty()) throw HospitalException("Role cannot be empty.");
    role = text;
}

void Staff::setDepartment(const std::string& value) {
    std::string text = Utils::clean(value);
    if (text.empty()) throw HospitalException("Department cannot be empty.");
    department = text;
}

void Staff::setSalary(double value) {
    if (value <= 0 || value > 1000000) throw HospitalException("Salary must be between 0.01 and 1000000.");
    salary = value;
}

void Staff::display() const {
    displayBasic();
    std::cout << "  Role          : " << role << "\n"
              << "  Department    : " << department << "\n"
              << "  Salary        : " << Utils::formatMoney(salary) << "\n";
}

void Staff::displaySummary() const {
    std::cout << "  [Staff] " << id << " - " << name << ", " << role << " (" << department << ")\n";
}

std::string Staff::toRecord() const {
    return basicRecord() + "|" + role + "|" + department + "|" + Utils::toFixed2(salary);
}

std::string Staff::getType() const { return "Staff"; }

Staff Staff::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 9) throw HospitalException("Staff record must have 9 fields.");
    char g = f[3].empty() ? ' ' : f[3][0];
    return Staff(Utils::toInt(f[0], "staff id"), f[1], Utils::toInt(f[2], "age"), g,
                 f[4], f[5], f[6], f[7], Utils::toDouble(f[8], "salary"));
}

int Staff::generateId() { return nextId++; }

void Staff::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
