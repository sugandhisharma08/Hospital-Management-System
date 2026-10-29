// HospitalRecords.cpp - Medical Records module (educational demo, not a real medical system)
#include "Hospital.h"
#include "Utils.h"
#include <iostream>

void HospitalSystem::recordsMenu() {
    if (currentRole == ROLE_RECEPTIONIST) {
        denyAccess();
        Utils::pause();
        return;
    }
    while (true) {
        Utils::printTitle("MEDICAL RECORDS");
        std::cout << "  (Educational demo with sample data - not a real medical records system)\n\n"
                  << "  1. Add visit record\n"
                  << "  2. View medical history of a patient\n"
                  << "  3. Search record by record ID\n"
                  << "  4. View all records\n"
                  << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 4);
        try {
            if (choice == 0) return;
            else if (choice == 1) addMedicalRecord();
            else if (choice == 2) viewRecordsByPatient();
            else if (choice == 3) searchRecordById();
            else if (choice == 4) viewAllRecords();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::addMedicalRecord() {
    Utils::printTitle("ADD VISIT RECORD");
    int pid = Utils::readInt("Patient ID: ", 1, 999999999);
    const Patient* patient = patients.findById(pid);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(pid) + ".");
    std::cout << "  Patient: " << patient->getName() << "\n";

    int did = Utils::readInt("Doctor ID: ", 1, 999999999);
    const Doctor* doctor = doctors.findById(did);
    if (doctor == nullptr) throw HospitalException("No doctor found with ID " + std::to_string(did) + ".");

    int aid = Utils::readInt("Related appointment ID (0 if none): ", 0, 999999999);
    if (aid > 0) {
        const Appointment* a = appointments.findById(aid);
        if (a == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(aid) + ".");
        if (a->getPatientId() != pid || a->getDoctorId() != did)
            throw HospitalException("That appointment belongs to a different patient or doctor.");
    }
    std::string date = Utils::readDate("Visit date (YYYY-MM-DD): ", false);
    if (date > Utils::todayDate()) throw HospitalException("A visit record cannot have a future date.");
    std::string diagnosis = Utils::readText("Diagnosis: ");
    std::string prescription = Utils::readText("Prescription notes (Enter to skip): ", true);
    std::string notes = Utils::readText("Other notes (Enter to skip): ", true);

    MedicalRecord record(MedicalRecord::generateId(), pid, did, aid, date, diagnosis, prescription, notes);
    records.addRecord(record);
    autoSave();
    activityLog.write("Added medical record " + std::to_string(record.getId()));
    std::cout << "\n  Record saved. Record ID: " << record.getId() << "\n";
}

// Used after an appointment is completed: patient, doctor and date are already known.
void HospitalSystem::addRecordForAppointment(const Appointment& appointment) {
    std::string date = appointment.getDate();
    if (date > Utils::todayDate()) date = Utils::todayDate();
    std::string diagnosis = Utils::readText("Diagnosis: ");
    std::string prescription = Utils::readText("Prescription notes (Enter to skip): ", true);
    std::string notes = Utils::readText("Other notes (Enter to skip): ", true);

    MedicalRecord record(MedicalRecord::generateId(), appointment.getPatientId(), appointment.getDoctorId(),
                         appointment.getId(), date, diagnosis, prescription, notes);
    records.addRecord(record);
    autoSave();
    activityLog.write("Added medical record " + std::to_string(record.getId()));
    std::cout << "\n  Record saved. Record ID: " << record.getId() << "\n";
}

void HospitalSystem::viewRecordsByPatient() const {
    int pid = Utils::readInt("Patient ID: ", 1, 999999999);
    const Patient* patient = patients.findById(pid);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(pid) + ".");
    std::vector<const MedicalRecord*> list = records.forPatient(pid);
    std::cout << "\n  Medical history of " << patient->getName() << " (ID " << pid << ")\n";
    if (list.empty()) {
        std::cout << "  No medical records found for this patient.\n";
        return;
    }
    for (std::size_t i = 0; i < list.size(); i++) {
        Utils::printLine('-', 60);
        list[i]->display();
        std::cout << "  Doctor name   : " << doctorName(list[i]->getDoctorId()) << "\n";
    }
    Utils::printLine('-', 60);
    std::cout << "  Total records: " << list.size() << "\n";
}

void HospitalSystem::searchRecordById() const {
    int id = Utils::readInt("Record ID: ", 1, 999999999);
    const MedicalRecord* r = records.findById(id);
    if (r == nullptr) throw HospitalException("No record found with ID " + std::to_string(id) + ".");
    std::cout << "\n";
    r->display();
    std::cout << "  Patient name  : " << patientName(r->getPatientId()) << "\n"
              << "  Doctor name   : " << doctorName(r->getDoctorId()) << "\n";
}

void HospitalSystem::viewAllRecords() const {
    if (records.count() == 0) {
        std::cout << "\n  No medical records have been added yet.\n";
        return;
    }
    const std::vector<MedicalRecord>& all = records.getAll();
    for (std::size_t i = 0; i < all.size(); i++) {
        Utils::printLine('-', 60);
        all[i].display();
        std::cout << "  Patient name  : " << patientName(all[i].getPatientId()) << "\n";
    }
    Utils::printLine('-', 60);
    std::cout << "  Total records: " << all.size() << "\n";
}
