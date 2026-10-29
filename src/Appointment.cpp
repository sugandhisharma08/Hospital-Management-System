// Appointment.cpp - implementation of the Appointment class
#include "Appointment.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

int Appointment::nextId = 3001;
const std::string Appointment::SCHEDULED = "Scheduled";
const std::string Appointment::COMPLETED = "Completed";
const std::string Appointment::CANCELLED = "Cancelled";

// The short constructor calls the full constructor (constructor delegation)
Appointment::Appointment(int appId, int pId, int dId, const std::string& appDate,
                         const std::string& appTime, const std::string& appReason)
    : Appointment(appId, pId, dId, appDate, appTime, appReason, "Scheduled") {}

Appointment::Appointment(int appId, int pId, int dId, const std::string& appDate,
                         const std::string& appTime, const std::string& appReason,
                         const std::string& appStatus)
    : id(appId), patientId(pId), doctorId(dId), date(""), timeSlot(""), reason(""), status("") {
    if (appId <= 0 || pId <= 0 || dId <= 0) throw HospitalException("Appointment IDs must be positive.");
    if (!Utils::isValidDate(appDate)) throw HospitalException("Invalid appointment date (use YYYY-MM-DD).");
    if (!Utils::isValidTime(appTime)) throw HospitalException("Invalid appointment time (use HH:MM).");
    std::string cleanReason = Utils::clean(appReason);
    if (cleanReason.empty()) throw HospitalException("Reason for visit cannot be empty.");
    if (appStatus != SCHEDULED && appStatus != COMPLETED && appStatus != CANCELLED)
        throw HospitalException("Invalid appointment status.");
    date = appDate;
    timeSlot = appTime;
    reason = cleanReason;
    status = appStatus;
}

int Appointment::getId() const { return id; }
int Appointment::getPatientId() const { return patientId; }
int Appointment::getDoctorId() const { return doctorId; }
std::string Appointment::getDate() const { return date; }
std::string Appointment::getTimeSlot() const { return timeSlot; }
std::string Appointment::getReason() const { return reason; }
std::string Appointment::getStatus() const { return status; }
bool Appointment::isScheduled() const { return status == SCHEDULED; }

void Appointment::reschedule(const std::string& newDate, const std::string& newTime) {
    if (status != SCHEDULED) throw HospitalException("Only scheduled appointments can be rescheduled.");
    if (!Utils::isValidDate(newDate)) throw HospitalException("Invalid new date.");
    if (!Utils::isValidTime(newTime)) throw HospitalException("Invalid new time.");
    date = newDate;
    timeSlot = newTime;
}

void Appointment::cancel() {
    if (status != SCHEDULED) throw HospitalException("Only scheduled appointments can be cancelled (current status: " + status + ").");
    status = CANCELLED;
}

void Appointment::complete() {
    if (status != SCHEDULED) throw HospitalException("Only scheduled appointments can be completed (current status: " + status + ").");
    status = COMPLETED;
}

void Appointment::display() const {
    std::cout << "  Appointment ID: " << id << "\n"
              << "  Patient ID    : " << patientId << "\n"
              << "  Doctor ID     : " << doctorId << "\n"
              << "  Date          : " << date << "\n"
              << "  Time          : " << timeSlot << "\n"
              << "  Status        : " << status << "\n"
              << "  Reason        : " << reason << "\n";
}

std::string Appointment::toRecord() const {
    std::ostringstream out;
    out << id << '|' << patientId << '|' << doctorId << '|' << date << '|' << timeSlot
        << '|' << status << '|' << reason;
    return out.str();
}

Appointment Appointment::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 7) throw HospitalException("Appointment record must have 7 fields.");
    return Appointment(Utils::toInt(f[0], "appointment id"), Utils::toInt(f[1], "patient id"),
                       Utils::toInt(f[2], "doctor id"), f[3], f[4], f[6], f[5]);
}

int Appointment::generateId() { return nextId++; }

void Appointment::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
