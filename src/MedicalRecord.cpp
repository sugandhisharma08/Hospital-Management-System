// MedicalRecord.cpp - implementation of the MedicalRecord class
#include "MedicalRecord.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

int MedicalRecord::nextId = 5001;

MedicalRecord::MedicalRecord(int recId, int pId, int dId, int appId, const std::string& date,
                             const std::string& diag, const std::string& prescr,
                             const std::string& note)
    : id(recId), patientId(pId), doctorId(dId), appointmentId(appId),
      visitDate(""), diagnosis(""), prescription(""), notes("") {
    if (recId <= 0 || pId <= 0 || dId <= 0) throw HospitalException("Record IDs must be positive.");
    if (appId < 0) throw HospitalException("Appointment ID cannot be negative.");
    if (!Utils::isValidDate(date)) throw HospitalException("Invalid visit date (use YYYY-MM-DD).");
    std::string d = Utils::clean(diag);
    if (d.empty()) throw HospitalException("Diagnosis cannot be empty.");
    std::string p = Utils::clean(prescr);
    std::string n = Utils::clean(note);
    visitDate = date;
    diagnosis = d;
    prescription = p.empty() ? "None" : p;
    notes = n.empty() ? "None" : n;
}

int MedicalRecord::getId() const { return id; }
int MedicalRecord::getPatientId() const { return patientId; }
int MedicalRecord::getDoctorId() const { return doctorId; }
int MedicalRecord::getAppointmentId() const { return appointmentId; }
std::string MedicalRecord::getVisitDate() const { return visitDate; }
std::string MedicalRecord::getDiagnosis() const { return diagnosis; }

void MedicalRecord::display() const {
    std::cout << "  Record ID     : " << id << "\n"
              << "  Patient ID    : " << patientId << "\n"
              << "  Doctor ID     : " << doctorId << "\n"
              << "  Appointment   : " << (appointmentId == 0 ? std::string("-") : std::to_string(appointmentId)) << "\n"
              << "  Visit Date    : " << visitDate << "\n"
              << "  Diagnosis     : " << diagnosis << "\n"
              << "  Prescription  : " << prescription << "\n"
              << "  Notes         : " << notes << "\n";
}

std::string MedicalRecord::toRecord() const {
    std::ostringstream out;
    out << id << '|' << patientId << '|' << doctorId << '|' << appointmentId << '|' << visitDate
        << '|' << diagnosis << '|' << prescription << '|' << notes;
    return out.str();
}

MedicalRecord MedicalRecord::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 8) throw HospitalException("Medical record must have 8 fields.");
    return MedicalRecord(Utils::toInt(f[0], "record id"), Utils::toInt(f[1], "patient id"),
                         Utils::toInt(f[2], "doctor id"), Utils::toInt(f[3], "appointment id"),
                         f[4], f[5], f[6], f[7]);
}

int MedicalRecord::generateId() { return nextId++; }

void MedicalRecord::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
