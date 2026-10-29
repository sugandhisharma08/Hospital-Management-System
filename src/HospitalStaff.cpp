// HospitalStaff.cpp - Staff and Administration module (administrator only)
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void HospitalSystem::staffMenu() {
    if (currentRole != ROLE_ADMIN) {
        denyAccess();
        Utils::pause();
        return;
    }
    while (true) {
        Utils::printTitle("STAFF MANAGEMENT (Administrator)");
        std::cout << "  1. Display all staff\n  2. Search staff (ID, name, role or department)\n"
                  << "  3. Register new staff member\n  4. Update staff member\n  5. Remove staff member\n"
                  << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 5);
        try {
            if (choice == 0) return;
            else if (choice == 1) displayAllStaff();
            else if (choice == 2) searchStaff();
            else if (choice == 3) registerStaff();
            else if (choice == 4) updateStaff();
            else if (choice == 5) deleteStaff();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::registerStaff() {
    Utils::printTitle("REGISTER NEW STAFF MEMBER");
    std::string name = Utils::readText("Full name: ");
    int age = Utils::readInt("Age (18-100): ", 18, 100);
    char gender = Utils::readGender("Gender (M/F/O): ");
    std::string contact = Utils::readPhone("Contact number (10 digits): ");
    std::string address = Utils::readText("Address: ");
    std::string role = Utils::readText("Role (Nurse, Receptionist, Pharmacist...): ");
    std::string department = Utils::readText("Department: ");
    double salary = Utils::readMoney("Monthly salary (Rs.): ", 0.01, 1000000);

    const std::vector<Staff>& all = staff.getAll();
    for (std::size_t i = 0; i < all.size(); i++) {
        if (Utils::toLower(all[i].getName()) == Utils::toLower(name) && all[i].getContact() == contact)
            throw HospitalException("This staff member is already registered (ID " + std::to_string(all[i].getId()) + ").");
    }

    Staff member(Staff::generateId(), name, age, gender, contact, address, role, department, salary);
    staff.addStaff(member);
    autoSave();
    activityLog.write("Registered staff " + std::to_string(member.getId()));
    std::cout << "\n  Staff member registered. Staff ID: " << member.getId() << "\n";
}

void HospitalSystem::showStaffList(const std::vector<const Staff*>& list) const {
    Utils::printLine('-', 80);
    std::cout << std::left << "  " << std::setw(7) << "ID" << std::setw(22) << "Name" << std::setw(18) << "Role"
              << std::setw(16) << "Department" << std::setw(12) << "Salary (Rs.)" << "\n";
    Utils::printLine('-', 80);
    for (std::size_t i = 0; i < list.size(); i++) {
        const Staff& s = *list[i];
        std::cout << "  " << std::setw(7) << s.getId() << std::setw(22) << Utils::fit(s.getName(), 21)
                  << std::setw(18) << Utils::fit(s.getRole(), 17) << std::setw(16) << Utils::fit(s.getDepartment(), 15)
                  << std::setw(12) << Utils::toFixed2(s.getSalary()) << "\n";
    }
    Utils::printLine('-', 80);
    std::cout << "  Total: " << list.size() << " staff member(s)\n";
}

void HospitalSystem::displayAllStaff() const {
    if (staff.count() == 0) {
        std::cout << "\n  No staff members are registered yet.\n";
        return;
    }
    std::vector<const Staff*> list;
    for (std::size_t i = 0; i < staff.getAll().size(); i++) list.push_back(&staff.getAll()[i]);
    showStaffList(list);
}

void HospitalSystem::searchStaff() const {
    std::string text = Utils::readText("Enter staff ID, name, role or department: ");
    bool allDigits = true;
    for (std::size_t i = 0; i < text.size(); i++)
        if (text[i] < '0' || text[i] > '9') allDigits = false;
    if (allDigits && text.size() <= 9) {
        const Staff* s = staff.findById(Utils::toInt(text, "staff id"));
        if (s == nullptr) throw HospitalException("No staff member found with ID " + text + ".");
        std::cout << "\n";
        s->display();
        return;
    }
    std::vector<const Staff*> found = staff.searchByText(text);
    if (found.empty()) throw HospitalException("No staff member found matching \"" + text + "\".");
    showStaffList(found);
}

void HospitalSystem::updateStaff() {
    Utils::printTitle("UPDATE STAFF MEMBER");
    int id = Utils::readInt("Enter staff ID: ", 1, 999999999);
    Staff* member = staff.findById(id);
    if (member == nullptr) throw HospitalException("No staff member found with ID " + std::to_string(id) + ".");
    member->display();
    std::cout << "\n  What do you want to change?\n"
              << "  1. Name   2. Contact   3. Address   4. Role   5. Department   6. Salary   0. Cancel\n";
    int field = Utils::readInt("Enter choice: ", 0, 6);
    if (field == 0) return;
    if (field == 1) member->setName(Utils::readText("New name: "));
    else if (field == 2) member->setContact(Utils::readPhone("New contact number (10 digits): "));
    else if (field == 3) member->setAddress(Utils::readText("New address: "));
    else if (field == 4) member->setRole(Utils::readText("New role: "));
    else if (field == 5) member->setDepartment(Utils::readText("New department: "));
    else if (field == 6) member->setSalary(Utils::readMoney("New salary (Rs.): ", 0.01, 1000000));
    autoSave();
    activityLog.write("Updated staff " + std::to_string(id));
    std::cout << "\n  Staff member updated successfully.\n";
}

void HospitalSystem::deleteStaff() {
    Utils::printTitle("REMOVE STAFF MEMBER");
    int id = Utils::readInt("Enter staff ID: ", 1, 999999999);
    const Staff* member = staff.findById(id);
    if (member == nullptr) throw HospitalException("No staff member found with ID " + std::to_string(id) + ".");
    member->display();
    if (!Utils::readYesNo("\n  Really remove this staff member? (y/n): ")) {
        std::cout << "  Removal cancelled.\n";
        return;
    }
    staff.removeStaff(id);
    autoSave();
    activityLog.write("Removed staff " + std::to_string(id));
    std::cout << "\n  Staff member removed.\n";
}
