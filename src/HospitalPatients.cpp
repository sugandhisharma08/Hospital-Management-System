// HospitalPatients.cpp - Patient Management module (menus and operations)
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void HospitalSystem::patientMenu() {
    bool canEdit = (currentRole != ROLE_DOCTOR);       // doctors can only view patients
    bool canDelete = (currentRole == ROLE_ADMIN);      // only the administrator can delete
    while (true) {
        Utils::printTitle("PATIENT MANAGEMENT");
        std::cout << "  1. Display all patients\n"
                  << "  2. Search patient by ID\n"
                  << "  3. Search patient by name\n";
        if (canEdit) std::cout << "  4. Register new patient\n  5. Update patient details\n";
        if (canDelete) std::cout << "  6. Delete patient\n";
        std::cout << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 6);
        try {
            if (choice == 0) return;
            else if (choice == 1) displayAllPatients();
            else if (choice == 2) searchPatient(Utils::readInt("Enter patient ID: ", 1, 999999999));
            else if (choice == 3) searchPatient(Utils::readText("Enter name (or part of it): "));
            else if (choice == 4 && canEdit) registerPatient();
            else if (choice == 5 && canEdit) updatePatient();
            else if (choice == 6 && canDelete) deletePatient();
            else denyAccess();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::registerPatient() {
    Utils::printTitle("REGISTER NEW PATIENT");
    std::string name = Utils::readText("Full name: ");
    int age = Utils::readInt("Age (0-120): ", 0, 120);
    char gender = Utils::readGender("Gender (M/F/O): ");
    std::string contact = Utils::readPhone("Contact number (10 digits): ");
    std::string address = Utils::readText("Address: ");

    std::string blood;
    while (true) {
        blood = Patient::normalizeBloodGroup(Utils::readLine("Blood group (A+ A- B+ B- AB+ AB- O+ O- or Unknown): "));
        if (!blood.empty()) break;
        std::cout << "  Invalid blood group.\n";
    }
    std::string emergency = Utils::readPhone("Emergency contact number (10 digits): ");

    // Extra duplicate protection: same name + same contact number is treated as the same person
    const std::vector<Patient>& all = patients.getAll();
    for (std::size_t i = 0; i < all.size(); i++) {
        if (Utils::toLower(all[i].getName()) == Utils::toLower(name) && all[i].getContact() == contact)
            throw HospitalException("A patient with the same name and contact number is already registered (ID " +
                                    std::to_string(all[i].getId()) + ").");
    }

    Patient patient(Patient::generateId(), name, age, gender, contact, address, blood, emergency, Utils::todayDate());
    patients.addPatient(patient);
    autoSave();
    activityLog.write("Registered patient " + std::to_string(patient.getId()));
    std::cout << "\n  Patient registered successfully. Patient ID: " << patient.getId() << "\n";
}

void HospitalSystem::searchPatient(int id) const {
    const Patient* patient = patients.findById(id);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(id) + ".");
    std::cout << "\n";
    patient->display();
    std::cout << "  ---------------------------------\n"
              << "  Appointments  : " << appointments.countForPatient(id) << "\n"
              << "  Medical recs  : " << records.countForPatient(id) << "\n"
              << "  Bills         : " << bills.countForPatient(id) << "\n";
}

void HospitalSystem::searchPatient(const std::string& name) const {
    std::vector<const Patient*> found = patients.searchByName(name);
    if (found.empty()) throw HospitalException("No patient found matching \"" + name + "\".");
    showPatientList(found);
}

void HospitalSystem::showPatientList(const std::vector<const Patient*>& list) const {
    Utils::printLine('-', 74);
    std::cout << std::left << "  " << std::setw(7) << "ID" << std::setw(24) << "Name" << std::setw(5) << "Age"
              << std::setw(8) << "Gender" << std::setw(13) << "Contact" << std::setw(7) << "Blood" << "\n";
    Utils::printLine('-', 74);
    for (std::size_t i = 0; i < list.size(); i++) {
        const Patient& p = *list[i];
        std::cout << "  " << std::setw(7) << p.getId() << std::setw(24) << Utils::fit(p.getName(), 23)
                  << std::setw(5) << p.getAge() << std::setw(8) << p.getGenderText()
                  << std::setw(13) << p.getContact() << std::setw(7) << p.getBloodGroup() << "\n";
    }
    Utils::printLine('-', 74);
    std::cout << "  Total: " << list.size() << " patient(s)\n";
}

void HospitalSystem::displayAllPatients() const {
    if (patients.count() == 0) {
        std::cout << "\n  No patients are registered yet.\n";
        return;
    }
    std::vector<const Patient*> list;
    for (std::size_t i = 0; i < patients.getAll().size(); i++) list.push_back(&patients.getAll()[i]);
    showPatientList(list);
}

void HospitalSystem::updatePatient() {
    Utils::printTitle("UPDATE PATIENT");
    int id = Utils::readInt("Enter patient ID: ", 1, 999999999);
    Patient* patient = patients.findById(id);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(id) + ".");
    patient->display();
    std::cout << "\n  What do you want to change?\n"
              << "  1. Name   2. Age   3. Gender   4. Contact   5. Address\n"
              << "  6. Blood group   7. Emergency contact   0. Cancel\n";
    int field = Utils::readInt("Enter choice: ", 0, 7);
    if (field == 0) return;
    if (field == 1) patient->setName(Utils::readText("New name: "));
    else if (field == 2) patient->setAge(Utils::readInt("New age (0-120): ", 0, 120));
    else if (field == 3) patient->setGender(Utils::readGender("New gender (M/F/O): "));
    else if (field == 4) patient->setContact(Utils::readPhone("New contact number (10 digits): "));
    else if (field == 5) patient->setAddress(Utils::readText("New address: "));
    else if (field == 6) patient->setBloodGroup(Utils::readLine("New blood group: "));
    else if (field == 7) patient->setEmergencyContact(Utils::readPhone("New emergency contact (10 digits): "));
    autoSave();
    activityLog.write("Updated patient " + std::to_string(id));
    std::cout << "\n  Patient updated successfully.\n";
}

// A patient who has appointments, medical records or bills is NOT deleted,
// because that would leave orphan records in the other modules.
void HospitalSystem::deletePatient() {
    Utils::printTitle("DELETE PATIENT");
    int id = Utils::readInt("Enter patient ID: ", 1, 999999999);
    const Patient* patient = patients.findById(id);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(id) + ".");
    int a = appointments.countForPatient(id);
    int r = records.countForPatient(id);
    int b = bills.countForPatient(id);
    if (a > 0 || r > 0 || b > 0) {
        throw HospitalException("Cannot delete " + patient->getName() + ": linked data exists (" +
                                std::to_string(a) + " appointments, " + std::to_string(r) + " records, " +
                                std::to_string(b) + " bills). This protects the other modules from broken links.");
    }
    patient->display();
    if (!Utils::readYesNo("\n  Really delete this patient? (y/n): ")) {
        std::cout << "  Deletion cancelled.\n";
        return;
    }
    patients.removePatient(id);
    autoSave();
    activityLog.write("Deleted patient " + std::to_string(id));
    std::cout << "\n  Patient deleted.\n";
}
