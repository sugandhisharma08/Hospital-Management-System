// Hospital.cpp - core of HospitalSystem: start-up, login, main menu, save/load, directory
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

HospitalSystem::HospitalSystem(const std::string& dataDirectory)
    : dataDir(dataDirectory),
      patients(dataDirectory + "/patients.txt"),
      doctors(dataDirectory + "/doctors.txt"),
      staff(dataDirectory + "/staff.txt"),
      appointments(dataDirectory + "/appointments.txt"),
      records(dataDirectory + "/records.txt"),
      bills(dataDirectory + "/bills.txt"),
      activityLog(),
      currentRole(ROLE_ADMIN) {
    activityLog.open(dataDir + "/activity_log.txt");
}

HospitalSystem::~HospitalSystem() {}

// ------------------------------------------------------------------
// Loading, saving and checking data
// ------------------------------------------------------------------
void HospitalSystem::loadAll() {
    int p = patients.load();
    int d = doctors.load();
    int s = staff.load();
    int a = appointments.load();
    int r = records.load();
    int b = bills.load();
    std::cout << "Data folder : " << dataDir << "\n"
              << "Loaded      : " << p << " patients, " << d << " doctors, " << s << " staff, "
              << a << " appointments, " << r << " records, " << b << " bills\n";
    int skipped = patients.getSkippedLines() + doctors.getSkippedLines() + staff.getSkippedLines() +
                  appointments.getSkippedLines() + records.getSkippedLines() + bills.getSkippedLines();
    if (skipped > 0)
        std::cout << "Warning     : " << skipped << " invalid or duplicate line(s) in the data files were skipped.\n";
    if (p + d + s + a + r + b == 0)
        std::cout << "Note        : no saved data found. A new empty database will be created automatically.\n";
    verifyDataIntegrity();
}

bool HospitalSystem::saveAll() {
    bool ok = true;
    ok = patients.save() && ok;
    ok = doctors.save() && ok;
    ok = staff.save() && ok;
    ok = appointments.save() && ok;
    ok = records.save() && ok;
    ok = bills.save() && ok;
    return ok;
}

void HospitalSystem::autoSave() {
    if (!saveAll())
        std::cout << "  WARNING: could not write the data files. Check that the '" << dataDir << "' folder is writable.\n";
}

// Reports (but does not delete) records that point to a patient or doctor that no longer exists.
void HospitalSystem::verifyDataIntegrity() const {
    int problems = 0;
    const std::vector<Appointment>& apps = appointments.getAll();
    for (std::size_t i = 0; i < apps.size(); i++) {
        if (!patients.exists(apps[i].getPatientId()) || !doctors.exists(apps[i].getDoctorId())) {
            std::cout << "Warning     : appointment " << apps[i].getId() << " refers to a missing patient or doctor.\n";
            problems++;
        }
    }
    const std::vector<MedicalRecord>& recs = records.getAll();
    for (std::size_t i = 0; i < recs.size(); i++) {
        if (!patients.exists(recs[i].getPatientId()) || !doctors.exists(recs[i].getDoctorId())) {
            std::cout << "Warning     : medical record " << recs[i].getId() << " refers to a missing patient or doctor.\n";
            problems++;
        }
    }
    const std::vector<Bill>& allBills = bills.getAll();
    for (std::size_t i = 0; i < allBills.size(); i++) {
        if (!patients.exists(allBills[i].getPatientId())) {
            std::cout << "Warning     : bill " << allBills[i].getId() << " refers to a missing patient.\n";
            problems++;
        }
    }
    if (problems == 0) std::cout << "Check       : all links between patients, doctors, appointments, records and bills are consistent.\n";
}

// ------------------------------------------------------------------
// Login and roles (a learning demo - NOT real security)
// ------------------------------------------------------------------
std::string HospitalSystem::expectedPassword() const {
    if (currentRole == ROLE_ADMIN) return "admin123";
    if (currentRole == ROLE_RECEPTIONIST) return "desk123";
    return "doctor123";
}

std::string HospitalSystem::roleName() const {
    if (currentRole == ROLE_ADMIN) return "Administrator";
    if (currentRole == ROLE_RECEPTIONIST) return "Receptionist";
    return "Doctor";
}

bool HospitalSystem::login() {
    std::cout << "\nLogin as:\n  1. Administrator\n  2. Receptionist\n  3. Doctor\n";
    int choice = Utils::readInt("Choose role (1-3): ", 1, 3);
    currentRole = static_cast<Role>(choice);
    for (int attempt = 1; attempt <= 3; attempt++) {
        std::string password = Utils::readLine("Password: ");
        if (password == expectedPassword()) return true;
        std::cout << "  Wrong password. Attempts left: " << (3 - attempt) << "\n";
    }
    return false;
}

void HospitalSystem::denyAccess() const {
    std::cout << "\n  Access denied: the " << roleName() << " role cannot use this option.\n";
}

// ------------------------------------------------------------------
// Main menu
// ------------------------------------------------------------------
std::vector<int> HospitalSystem::menuActionsForRole() const {
    std::vector<int> actions;
    actions.push_back(ACT_PATIENTS);
    actions.push_back(ACT_DOCTORS);
    if (currentRole == ROLE_ADMIN) actions.push_back(ACT_STAFF);
    actions.push_back(ACT_APPOINTMENTS);
    if (currentRole != ROLE_RECEPTIONIST) actions.push_back(ACT_RECORDS);
    if (currentRole != ROLE_DOCTOR) actions.push_back(ACT_BILLING);
    actions.push_back(ACT_REPORTS);
    if (currentRole == ROLE_ADMIN) {
        actions.push_back(ACT_DIRECTORY);
        actions.push_back(ACT_ACTIVITY_LOG);
    }
    actions.push_back(ACT_SAVE);
    return actions;
}

std::string HospitalSystem::actionLabel(int action) const {
    switch (action) {
        case ACT_PATIENTS:     return "Patient Management";
        case ACT_DOCTORS:      return "Doctor Management";
        case ACT_STAFF:        return "Staff Management";
        case ACT_APPOINTMENTS: return "Appointment Management";
        case ACT_RECORDS:      return "Medical Records";
        case ACT_BILLING:      return "Billing and Payments";
        case ACT_REPORTS:      return "Dashboard and Reports";
        case ACT_DIRECTORY:    return "Hospital Directory (all people)";
        case ACT_ACTIVITY_LOG: return "View Activity Log";
        case ACT_SAVE:         return "Save All Data Now";
        default:               return "Unknown";
    }
}

void HospitalSystem::handleAction(int action) {
    switch (action) {
        case ACT_PATIENTS:     patientMenu(); break;
        case ACT_DOCTORS:      doctorMenu(); break;
        case ACT_STAFF:        staffMenu(); break;
        case ACT_APPOINTMENTS: appointmentMenu(); break;
        case ACT_RECORDS:      recordsMenu(); break;
        case ACT_BILLING:      billingMenu(); break;
        case ACT_REPORTS:      reportsMenu(); break;
        case ACT_DIRECTORY:    showDirectory(); Utils::pause(); break;
        case ACT_ACTIVITY_LOG: showActivityLog(); Utils::pause(); break;
        case ACT_SAVE:
            if (saveAll()) std::cout << "\n  All data saved to the '" << dataDir << "' folder.\n";
            else std::cout << "\n  ERROR: could not save data. Check the folder permissions.\n";
            Utils::pause();
            break;
        default: break;
    }
}

void HospitalSystem::run() {
    Utils::printTitle("ADVANCED HOSPITAL MANAGEMENT SYSTEM (C++ OOP Project)");
    std::cout << "Educational project. Sample data only - not for real medical use.\n\n";
    loadAll();
    if (!login()) {
        std::cout << "\nToo many wrong passwords. Exiting.\n";
        return;
    }
    activityLog.write("Login as " + roleName());
    std::cout << "\nWelcome, " << roleName() << "!\n";

    bool running = true;
    while (running) {
        std::vector<int> actions = menuActionsForRole();
        Utils::printTitle("MAIN MENU - " + roleName());
        for (std::size_t i = 0; i < actions.size(); i++)
            std::cout << "  " << (i + 1) << ". " << actionLabel(actions[i]) << "\n";
        std::cout << "  0. Save and Exit\n";
        int choice = Utils::readInt("Enter choice: ", 0, static_cast<int>(actions.size()));
        if (choice == 0) running = false;
        else handleAction(actions[choice - 1]);
    }
    autoSave();
    activityLog.write("Logout (" + roleName() + ")");
    std::cout << "\nAll data saved. Goodbye!\n";
}

// ------------------------------------------------------------------
// Small helpers
// ------------------------------------------------------------------
std::string HospitalSystem::patientName(int id) const {
    const Patient* p = patients.findById(id);
    return (p != nullptr) ? p->getName() : "(missing)";
}

std::string HospitalSystem::doctorName(int id) const {
    const Doctor* d = doctors.findById(id);
    return (d != nullptr) ? d->getName() : "(missing)";
}

// POLYMORPHISM demo: one list of Person pointers holds patients, doctors and staff.
// displaySummary() is virtual, so each object prints itself in its own way.
void HospitalSystem::showDirectory() const {
    Utils::printTitle("HOSPITAL DIRECTORY (polymorphism demo)");
    std::vector<const Person*> everyone;
    for (std::size_t i = 0; i < patients.getAll().size(); i++) everyone.push_back(&patients.getAll()[i]);
    for (std::size_t i = 0; i < doctors.getAll().size(); i++) everyone.push_back(&doctors.getAll()[i]);
    for (std::size_t i = 0; i < staff.getAll().size(); i++) everyone.push_back(&staff.getAll()[i]);
    if (everyone.empty()) {
        std::cout << "  Nobody is registered yet.\n";
        return;
    }
    for (std::size_t i = 0; i < everyone.size(); i++) everyone[i]->displaySummary();
    std::cout << "\n  Total people: " << everyone.size() << "\n";
}

void HospitalSystem::showActivityLog() const {
    Utils::printTitle("ACTIVITY LOG (last 20 entries)");
    std::vector<std::string> lines = activityLog.readLastLines(20);
    if (lines.empty()) std::cout << "  The log is empty.\n";
    for (std::size_t i = 0; i < lines.size(); i++) std::cout << "  " << lines[i] << "\n";
}
