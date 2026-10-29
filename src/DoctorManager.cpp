// DoctorManager.cpp
#include "DoctorManager.h"
#include "Utils.h"

DoctorManager::DoctorManager(const std::string& path) : DataManager(path) {}

bool DoctorManager::parseLine(const std::string& line) {
    Doctor d = Doctor::fromRecord(line);
    if (exists(d.getId())) return false;
    doctors.push_back(d);
    Doctor::syncNextId(d.getId());
    return true;
}

void DoctorManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < doctors.size(); i++) out << doctors[i].toRecord() << "\n";
}

int DoctorManager::count() const { return static_cast<int>(doctors.size()); }

bool DoctorManager::exists(int id) const { return findById(id) != nullptr; }

Doctor* DoctorManager::findById(int id) {
    for (std::size_t i = 0; i < doctors.size(); i++)
        if (doctors[i].getId() == id) return &doctors[i];
    return nullptr;
}

const Doctor* DoctorManager::findById(int id) const {
    for (std::size_t i = 0; i < doctors.size(); i++)
        if (doctors[i].getId() == id) return &doctors[i];
    return nullptr;
}

std::vector<const Doctor*> DoctorManager::searchByText(const std::string& text) const {
    std::vector<const Doctor*> result;
    for (std::size_t i = 0; i < doctors.size(); i++) {
        if (Utils::containsIgnoreCase(doctors[i].getName(), text) ||
            Utils::containsIgnoreCase(doctors[i].getSpecialization(), text))
            result.push_back(&doctors[i]);
    }
    return result;
}

std::vector<const Doctor*> DoctorManager::getAvailable() const {
    std::vector<const Doctor*> result;
    for (std::size_t i = 0; i < doctors.size(); i++)
        if (doctors[i].isAvailable()) result.push_back(&doctors[i]);
    return result;
}

const std::vector<Doctor>& DoctorManager::getAll() const { return doctors; }

void DoctorManager::addDoctor(const Doctor& doctor) {
    if (exists(doctor.getId())) throw HospitalException("Doctor ID already exists.");
    doctors.push_back(doctor);
    Doctor::syncNextId(doctor.getId());
}

void DoctorManager::removeDoctor(int id) {
    for (std::size_t i = 0; i < doctors.size(); i++) {
        if (doctors[i].getId() == id) {
            doctors.erase(doctors.begin() + static_cast<std::ptrdiff_t>(i));
            return;
        }
    }
    throw HospitalException("Doctor not found.");
}

std::string DoctorManager::moduleName() const { return "Doctors"; }
