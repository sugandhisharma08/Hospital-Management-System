// PatientManager.cpp
#include "PatientManager.h"
#include "Utils.h"

PatientManager::PatientManager(const std::string& path) : DataManager(path) {}

bool PatientManager::parseLine(const std::string& line) {
    Patient p = Patient::fromRecord(line);
    if (exists(p.getId())) return false;          // duplicate ID in the file: skip it
    patients.push_back(p);
    Patient::syncNextId(p.getId());
    return true;
}

void PatientManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < patients.size(); i++) out << patients[i].toRecord() << "\n";
}

int PatientManager::count() const { return static_cast<int>(patients.size()); }

bool PatientManager::exists(int id) const { return findById(id) != nullptr; }

Patient* PatientManager::findById(int id) {
    for (std::size_t i = 0; i < patients.size(); i++)
        if (patients[i].getId() == id) return &patients[i];
    return nullptr;
}

const Patient* PatientManager::findById(int id) const {
    for (std::size_t i = 0; i < patients.size(); i++)
        if (patients[i].getId() == id) return &patients[i];
    return nullptr;
}

std::vector<const Patient*> PatientManager::searchByName(const std::string& text) const {
    std::vector<const Patient*> result;
    for (std::size_t i = 0; i < patients.size(); i++)
        if (Utils::containsIgnoreCase(patients[i].getName(), text)) result.push_back(&patients[i]);
    return result;
}

const std::vector<Patient>& PatientManager::getAll() const { return patients; }

void PatientManager::addPatient(const Patient& patient) {
    if (exists(patient.getId())) throw HospitalException("Patient ID already exists.");
    patients.push_back(patient);
    Patient::syncNextId(patient.getId());
}

void PatientManager::removePatient(int id) {
    for (std::size_t i = 0; i < patients.size(); i++) {
        if (patients[i].getId() == id) {
            patients.erase(patients.begin() + static_cast<std::ptrdiff_t>(i));
            return;
        }
    }
    throw HospitalException("Patient not found.");
}

std::string PatientManager::moduleName() const { return "Patients"; }
