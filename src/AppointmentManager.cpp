// AppointmentManager.cpp
#include "AppointmentManager.h"
#include "Utils.h"

AppointmentManager::AppointmentManager(const std::string& path) : DataManager(path) {}

bool AppointmentManager::parseLine(const std::string& line) {
    Appointment a = Appointment::fromRecord(line);
    if (exists(a.getId())) return false;
    appointments.push_back(a);
    Appointment::syncNextId(a.getId());
    return true;
}

void AppointmentManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < appointments.size(); i++) out << appointments[i].toRecord() << "\n";
}

int AppointmentManager::count() const { return static_cast<int>(appointments.size()); }

bool AppointmentManager::exists(int id) const { return findById(id) != nullptr; }

Appointment* AppointmentManager::findById(int id) {
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getId() == id) return &appointments[i];
    return nullptr;
}

const Appointment* AppointmentManager::findById(int id) const {
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getId() == id) return &appointments[i];
    return nullptr;
}

const std::vector<Appointment>& AppointmentManager::getAll() const { return appointments; }

void AppointmentManager::addAppointment(const Appointment& appointment) {
    if (exists(appointment.getId())) throw HospitalException("Appointment ID already exists.");
    appointments.push_back(appointment);
    Appointment::syncNextId(appointment.getId());
}

void AppointmentManager::checkSlotFree(int doctorId, int patientId, const std::string& date,
                                       const std::string& time, int ignoreId) const {
    for (std::size_t i = 0; i < appointments.size(); i++) {
        const Appointment& a = appointments[i];
        if (a.getId() == ignoreId || !a.isScheduled()) continue;
        if (a.getDate() != date || a.getTimeSlot() != time) continue;
        if (a.getDoctorId() == doctorId)
            throw HospitalException("The doctor already has an appointment on " + date + " at " + time + ".");
        if (a.getPatientId() == patientId)
            throw HospitalException("The patient already has an appointment on " + date + " at " + time + ".");
    }
}

std::vector<const Appointment*> AppointmentManager::forPatient(int patientId) const {
    std::vector<const Appointment*> result;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getPatientId() == patientId) result.push_back(&appointments[i]);
    return result;
}

std::vector<const Appointment*> AppointmentManager::withStatus(const std::string& status) const {
    std::vector<const Appointment*> result;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getStatus() == status) result.push_back(&appointments[i]);
    return result;
}

int AppointmentManager::countForPatient(int patientId) const {
    int total = 0;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getPatientId() == patientId) total++;
    return total;
}

int AppointmentManager::countForDoctor(int doctorId) const {
    int total = 0;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getDoctorId() == doctorId) total++;
    return total;
}

int AppointmentManager::countForDoctor(int doctorId, const std::string& status) const {
    int total = 0;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getDoctorId() == doctorId && appointments[i].getStatus() == status) total++;
    return total;
}

int AppointmentManager::countByStatus(const std::string& status) const {
    int total = 0;
    for (std::size_t i = 0; i < appointments.size(); i++)
        if (appointments[i].getStatus() == status) total++;
    return total;
}

std::string AppointmentManager::moduleName() const { return "Appointments"; }
