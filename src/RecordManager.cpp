// RecordManager.cpp
#include "RecordManager.h"
#include "Utils.h"

RecordManager::RecordManager(const std::string& path) : DataManager(path) {}

bool RecordManager::parseLine(const std::string& line) {
    MedicalRecord r = MedicalRecord::fromRecord(line);
    if (exists(r.getId())) return false;
    records.push_back(r);
    MedicalRecord::syncNextId(r.getId());
    return true;
}

void RecordManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < records.size(); i++) out << records[i].toRecord() << "\n";
}

int RecordManager::count() const { return static_cast<int>(records.size()); }

bool RecordManager::exists(int id) const { return findById(id) != nullptr; }

const MedicalRecord* RecordManager::findById(int id) const {
    for (std::size_t i = 0; i < records.size(); i++)
        if (records[i].getId() == id) return &records[i];
    return nullptr;
}

const std::vector<MedicalRecord>& RecordManager::getAll() const { return records; }

void RecordManager::addRecord(const MedicalRecord& record) {
    if (exists(record.getId())) throw HospitalException("Record ID already exists.");
    records.push_back(record);
    MedicalRecord::syncNextId(record.getId());
}

std::vector<const MedicalRecord*> RecordManager::forPatient(int patientId) const {
    std::vector<const MedicalRecord*> result;
    for (std::size_t i = 0; i < records.size(); i++)
        if (records[i].getPatientId() == patientId) result.push_back(&records[i]);
    return result;
}

int RecordManager::countForPatient(int patientId) const {
    int total = 0;
    for (std::size_t i = 0; i < records.size(); i++)
        if (records[i].getPatientId() == patientId) total++;
    return total;
}

int RecordManager::countForDoctor(int doctorId) const {
    int total = 0;
    for (std::size_t i = 0; i < records.size(); i++)
        if (records[i].getDoctorId() == doctorId) total++;
    return total;
}

bool RecordManager::hasRecordForAppointment(int appointmentId) const {
    for (std::size_t i = 0; i < records.size(); i++)
        if (records[i].getAppointmentId() == appointmentId) return true;
    return false;
}

std::string RecordManager::moduleName() const { return "Medical Records"; }
