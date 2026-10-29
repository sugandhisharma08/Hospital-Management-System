// Patient.cpp - implementation of the Patient class
#include "Patient.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <cctype>

int Patient::nextId = 1001;      // first patient ID

Patient::Patient(int pid, const std::string& pname, int page, char pgender,
                 const std::string& pcontact, const std::string& paddress,
                 const std::string& blood, const std::string& emergency,
                 const std::string& regDate)
    : Person(pid, pname, page, pgender, pcontact, paddress),
      bloodGroup("Unknown"), emergencyContact(""), registeredDate("") {
    setBloodGroup(blood);
    setEmergencyContact(emergency);
    if (!Utils::isValidDate(regDate)) throw HospitalException("Invalid registration date.");
    registeredDate = regDate;
}

Patient::~Patient() {}

std::string Patient::getBloodGroup() const { return bloodGroup; }
std::string Patient::getEmergencyContact() const { return emergencyContact; }
std::string Patient::getRegisteredDate() const { return registeredDate; }

std::string Patient::normalizeBloodGroup(const std::string& text) {
    std::string v = Utils::trim(text);
    for (std::size_t i = 0; i < v.size(); i++)
        v[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(v[i])));
    if (v == "UNKNOWN") return "Unknown";
    const char* valid[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (int i = 0; i < 8; i++)
        if (v == valid[i]) return v;
    return "";
}

void Patient::setBloodGroup(const std::string& value) {
    std::string v = normalizeBloodGroup(value);
    if (v.empty()) throw HospitalException("Blood group must be A+, A-, B+, B-, AB+, AB-, O+, O- or Unknown.");
    bloodGroup = v;
}

void Patient::setEmergencyContact(const std::string& value) {
    if (!Utils::isValidPhone(value)) throw HospitalException("Emergency contact must contain exactly 10 digits.");
    emergencyContact = value;
}

void Patient::display() const {
    displayBasic();
    std::cout << "  Blood Group   : " << bloodGroup << "\n"
              << "  Emergency No. : " << emergencyContact << "\n"
              << "  Registered On : " << registeredDate << "\n";
}

void Patient::displaySummary() const {
    std::cout << "  [Patient] " << id << " - " << name << ", " << age << " yrs, blood group " << bloodGroup << "\n";
}

std::string Patient::toRecord() const {
    return basicRecord() + "|" + bloodGroup + "|" + emergencyContact + "|" + registeredDate;
}

std::string Patient::getType() const { return "Patient"; }

Patient Patient::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 9) throw HospitalException("Patient record must have 9 fields.");
    char g = f[3].empty() ? ' ' : f[3][0];
    return Patient(Utils::toInt(f[0], "patient id"), f[1], Utils::toInt(f[2], "age"), g,
                   f[4], f[5], f[6], f[7], f[8]);
}

int Patient::generateId() { return nextId++; }

void Patient::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
