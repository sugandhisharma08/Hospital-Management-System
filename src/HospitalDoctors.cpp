// HospitalDoctors.cpp - Doctor Management module
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void HospitalSystem::doctorMenu() {
    bool canEdit = (currentRole == ROLE_ADMIN);        // only the administrator changes doctor data
    while (true) {
        Utils::printTitle("DOCTOR MANAGEMENT");
        std::cout << "  1. Display all doctors\n"
                  << "  2. Search doctor (by ID, name or specialization)\n"
                  << "  3. Show available doctors\n";
        if (canEdit) std::cout << "  4. Register new doctor\n  5. Update doctor\n  6. Remove doctor\n";
        std::cout << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 6);
        try {
            if (choice == 0) return;
            else if (choice == 1) displayAllDoctors();
            else if (choice == 2) searchDoctors();
            else if (choice == 3) showAvailableDoctors();
            else if (choice == 4 && canEdit) registerDoctor();
            else if (choice == 5 && canEdit) updateDoctor();
            else if (choice == 6 && canEdit) deleteDoctor();
            else denyAccess();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::registerDoctor() {
    Utils::printTitle("REGISTER NEW DOCTOR");
    std::string name = Utils::readText("Doctor name (example Dr. Anil Rao): ");
    int age = Utils::readInt("Age (20-100): ", 20, 100);
    char gender = Utils::readGender("Gender (M/F/O): ");
    std::string contact = Utils::readPhone("Contact number (10 digits): ");
    std::string address = Utils::readText("Address: ");
    std::string specialization = Utils::readText("Specialization: ");
    double fee = Utils::readMoney("Consultation fee (Rs.): ", 0.01, 100000);
    std::string start = Utils::readTime("Shift start time (HH:MM): ");
    std::string end = Utils::readTime("Shift end time (HH:MM): ");

    const std::vector<Doctor>& all = doctors.getAll();
    for (std::size_t i = 0; i < all.size(); i++) {
        if (Utils::toLower(all[i].getName()) == Utils::toLower(name) && all[i].getContact() == contact)
            throw HospitalException("This doctor is already registered (ID " + std::to_string(all[i].getId()) + ").");
    }

    Doctor doctor(Doctor::generateId(), name, age, gender, contact, address, specialization, fee, start, end, false);
    doctors.addDoctor(doctor);
    autoSave();
    activityLog.write("Registered doctor " + std::to_string(doctor.getId()));
    std::cout << "\n  Doctor registered successfully. Doctor ID: " << doctor.getId() << "\n";
}

void HospitalSystem::showDoctorList(const std::vector<const Doctor*>& list) const {
    Utils::printLine('-', 86);
    std::cout << std::left << "  " << std::setw(7) << "ID" << std::setw(22) << "Name" << std::setw(18) << "Specialization"
              << std::setw(12) << "Fee (Rs.)" << std::setw(13) << "Shift" << std::setw(9) << "Status" << "\n";
    Utils::printLine('-', 86);
    for (std::size_t i = 0; i < list.size(); i++) {
        const Doctor& d = *list[i];
        std::cout << "  " << std::setw(7) << d.getId() << std::setw(22) << Utils::fit(d.getName(), 21)
                  << std::setw(18) << Utils::fit(d.getSpecialization(), 17)
                  << std::setw(12) << Utils::toFixed2(d.getFee())
                  << std::setw(13) << (d.getShiftStart() + "-" + d.getShiftEnd())
                  << std::setw(9) << (d.isOnLeave() ? "On leave" : "Available") << "\n";
    }
    Utils::printLine('-', 86);
    std::cout << "  Total: " << list.size() << " doctor(s)\n";
}

void HospitalSystem::displayAllDoctors() const {
    if (doctors.count() == 0) {
        std::cout << "\n  No doctors are registered yet.\n";
        return;
    }
    std::vector<const Doctor*> list;
    for (std::size_t i = 0; i < doctors.getAll().size(); i++) list.push_back(&doctors.getAll()[i]);
    showDoctorList(list);
}

void HospitalSystem::showAvailableDoctors() const {
    std::vector<const Doctor*> list = doctors.getAvailable();
    if (list.empty()) {
        std::cout << "\n  No doctors are available right now.\n";
        return;
    }
    showDoctorList(list);
}

void HospitalSystem::searchDoctors() const {
    std::string text = Utils::readText("Enter doctor ID, name or specialization: ");
    bool allDigits = true;
    for (std::size_t i = 0; i < text.size(); i++)
        if (text[i] < '0' || text[i] > '9') allDigits = false;
    if (allDigits && text.size() <= 9) {
        const Doctor* d = doctors.findById(Utils::toInt(text, "doctor id"));
        if (d == nullptr) throw HospitalException("No doctor found with ID " + text + ".");
        std::cout << "\n";
        d->display();
        std::cout << "  Appointments  : " << appointments.countForDoctor(d->getId()) << "\n";
        return;
    }
    std::vector<const Doctor*> found = doctors.searchByText(text);
    if (found.empty()) throw HospitalException("No doctor found matching \"" + text + "\".");
    showDoctorList(found);
}

void HospitalSystem::updateDoctor() {
    Utils::printTitle("UPDATE DOCTOR");
    int id = Utils::readInt("Enter doctor ID: ", 1, 999999999);
    Doctor* doctor = doctors.findById(id);
    if (doctor == nullptr) throw HospitalException("No doctor found with ID " + std::to_string(id) + ".");
    doctor->display();
    std::cout << "\n  What do you want to change?\n"
              << "  1. Name   2. Contact   3. Specialization   4. Consultation fee\n"
              << "  5. Shift timing   6. Toggle leave status   0. Cancel\n";
    int field = Utils::readInt("Enter choice: ", 0, 6);
    if (field == 0) return;
    if (field == 1) doctor->setName(Utils::readText("New name: "));
    else if (field == 2) doctor->setContact(Utils::readPhone("New contact number (10 digits): "));
    else if (field == 3) doctor->setSpecialization(Utils::readText("New specialization: "));
    else if (field == 4) doctor->setFee(Utils::readMoney("New fee (Rs.): ", 0.01, 100000));
    else if (field == 5) {
        std::string start = Utils::readTime("New shift start (HH:MM): ");
        std::string end = Utils::readTime("New shift end (HH:MM): ");
        doctor->setShift(start, end);
    } else if (field == 6) {
        doctor->setOnLeave(!doctor->isOnLeave());
        std::cout << "  Doctor is now " << (doctor->isOnLeave() ? "ON LEAVE" : "AVAILABLE") << ".\n";
    }
    autoSave();
    activityLog.write("Updated doctor " + std::to_string(id));
    std::cout << "\n  Doctor updated successfully.\n";
}

void HospitalSystem::deleteDoctor() {
    Utils::printTitle("REMOVE DOCTOR");
    int id = Utils::readInt("Enter doctor ID: ", 1, 999999999);
    const Doctor* doctor = doctors.findById(id);
    if (doctor == nullptr) throw HospitalException("No doctor found with ID " + std::to_string(id) + ".");
    int a = appointments.countForDoctor(id);
    int r = records.countForDoctor(id);
    if (a > 0 || r > 0) {
        throw HospitalException("Cannot remove " + doctor->getName() + ": linked data exists (" +
                                std::to_string(a) + " appointments, " + std::to_string(r) +
                                " records). Mark the doctor as 'On leave' instead.");
    }
    doctor->display();
    if (!Utils::readYesNo("\n  Really remove this doctor? (y/n): ")) {
        std::cout << "  Removal cancelled.\n";
        return;
    }
    doctors.removeDoctor(id);
    autoSave();
    activityLog.write("Removed doctor " + std::to_string(id));
    std::cout << "\n  Doctor removed.\n";
}
