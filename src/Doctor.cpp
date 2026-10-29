// Doctor.cpp - implementation of the Doctor class
#include "Doctor.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

int Doctor::nextId = 2001;

Doctor::Doctor(int did, const std::string& dname, int dage, char dgender,
               const std::string& dcontact, const std::string& daddress,
               const std::string& dspecialization, double dfee,
               const std::string& start, const std::string& end, bool leave)
    : Person(did, dname, dage, dgender, dcontact, daddress),
      specialization(""), fee(0), shiftStart("09:00"), shiftEnd("17:00"), onLeave(leave) {
    setSpecialization(dspecialization);
    setFee(dfee);
    setShift(start, end);
}

Doctor::~Doctor() {}

std::string Doctor::getSpecialization() const { return specialization; }
double Doctor::getFee() const { return fee; }
std::string Doctor::getShiftStart() const { return shiftStart; }
std::string Doctor::getShiftEnd() const { return shiftEnd; }
bool Doctor::isOnLeave() const { return onLeave; }

void Doctor::setSpecialization(const std::string& value) {
    std::string text = Utils::clean(value);
    if (text.empty()) throw HospitalException("Specialization cannot be empty.");
    specialization = text;
}

void Doctor::setFee(double value) {
    if (value <= 0 || value > 100000) throw HospitalException("Consultation fee must be between 0.01 and 100000.");
    fee = value;
}

void Doctor::setShift(const std::string& start, const std::string& end) {
    if (!Utils::isValidTime(start) || !Utils::isValidTime(end))
        throw HospitalException("Shift times must be in HH:MM format.");
    if (Utils::timeToMinutes(start) >= Utils::timeToMinutes(end))
        throw HospitalException("Shift start time must be earlier than the end time.");
    shiftStart = start;
    shiftEnd = end;
}

void Doctor::setOnLeave(bool value) { onLeave = value; }

bool Doctor::isAvailable() const { return !onLeave; }

bool Doctor::isAvailable(const std::string& time) const {
    if (onLeave || !Utils::isValidTime(time)) return false;
    int t = Utils::timeToMinutes(time);
    return t >= Utils::timeToMinutes(shiftStart) && t < Utils::timeToMinutes(shiftEnd);
}

void Doctor::display() const {
    displayBasic();
    std::cout << "  Specialization: " << specialization << "\n"
              << "  Consult. Fee  : " << Utils::formatMoney(fee) << "\n"
              << "  Shift         : " << shiftStart << " - " << shiftEnd << "\n"
              << "  Status        : " << (onLeave ? "On leave" : "Available") << "\n";
}

void Doctor::displaySummary() const {
    std::cout << "  [Doctor] " << id << " - " << name << ", " << specialization
              << ", fee " << Utils::formatMoney(fee) << "\n";
}

std::string Doctor::toRecord() const {
    return basicRecord() + "|" + specialization + "|" + Utils::toFixed2(fee) + "|" +
           shiftStart + "|" + shiftEnd + "|" + (onLeave ? "1" : "0");
}

std::string Doctor::getType() const { return "Doctor"; }

Doctor Doctor::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 11) throw HospitalException("Doctor record must have 11 fields.");
    char g = f[3].empty() ? ' ' : f[3][0];
    return Doctor(Utils::toInt(f[0], "doctor id"), f[1], Utils::toInt(f[2], "age"), g,
                  f[4], f[5], f[6], Utils::toDouble(f[7], "fee"), f[8], f[9], f[10] == "1");
}

int Doctor::generateId() { return nextId++; }

void Doctor::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
