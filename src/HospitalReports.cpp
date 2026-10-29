// HospitalReports.cpp - Dashboard and Reports module
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// Comparison function used by std::sort: earlier date/time first
static bool earlierAppointment(const Appointment* a, const Appointment* b) {
    if (a->getDate() != b->getDate()) return a->getDate() < b->getDate();
    return a->getTimeSlot() < b->getTimeSlot();
}

void HospitalSystem::reportsMenu() {
    while (true) {
        Utils::printTitle("DASHBOARD AND REPORTS");
        std::cout << "  1. Hospital dashboard (summary)\n"
                  << "  2. Pending appointments\n"
                  << "  3. Unpaid bills\n"
                  << "  4. Doctor workload\n"
                  << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 4);
        if (choice == 0) return;
        if (choice == 3 && currentRole == ROLE_DOCTOR) {
            denyAccess();
        } else {
            if (choice == 1) showDashboard();
            else if (choice == 2) showPendingAppointments();
            else if (choice == 3) showUnpaidBills();
            else if (choice == 4) showDoctorWorkload();
        }
        Utils::pause();
    }
}

void HospitalSystem::showDashboard() const {
    Utils::printTitle("HOSPITAL DASHBOARD  -  " + Utils::todayDate());
    std::cout << "  Registered patients   : " << patients.count() << "\n"
              << "  Registered doctors    : " << doctors.count() << "\n"
              << "  Staff members         : " << staff.count() << "\n"
              << "  Total appointments    : " << appointments.count() << "\n"
              << "     Scheduled (pending): " << appointments.countByStatus(Appointment::SCHEDULED) << "\n"
              << "     Completed          : " << appointments.countByStatus(Appointment::COMPLETED) << "\n"
              << "     Cancelled          : " << appointments.countByStatus(Appointment::CANCELLED) << "\n"
              << "  Medical records       : " << records.count() << "\n";
    if (currentRole != ROLE_DOCTOR) {
        std::cout << "  Total bills           : " << bills.count() << "\n"
                  << "     Unpaid bills       : " << bills.countUnpaid() << "\n"
                  << "     Outstanding amount : " << Utils::formatMoney(bills.totalOutstanding()) << "\n"
                  << "     Amount collected   : " << Utils::formatMoney(bills.totalCollected()) << "\n";
    }
    std::cout << "\n  --- Pending appointments ---\n";
    showPendingAppointments();
    if (currentRole != ROLE_DOCTOR) {
        std::cout << "\n  --- Unpaid bills ---\n";
        showUnpaidBills();
    }
}

void HospitalSystem::showPendingAppointments() const {
    std::vector<const Appointment*> list = appointments.withStatus(Appointment::SCHEDULED);
    if (list.empty()) {
        std::cout << "\n  There are no pending appointments.\n";
        return;
    }
    std::sort(list.begin(), list.end(), earlierAppointment);
    showAppointmentList(list);
}

void HospitalSystem::showDoctorWorkload() const {
    if (doctors.count() == 0) {
        std::cout << "\n  No doctors are registered yet.\n";
        return;
    }
    Utils::printLine('-', 70);
    std::cout << std::left << "  " << std::setw(7) << "ID" << std::setw(24) << "Doctor" << std::setw(8) << "Total"
              << std::setw(11) << "Scheduled" << std::setw(11) << "Completed" << std::setw(10) << "Cancelled" << "\n";
    Utils::printLine('-', 70);
    const std::vector<Doctor>& all = doctors.getAll();
    for (std::size_t i = 0; i < all.size(); i++) {
        int id = all[i].getId();
        std::cout << "  " << std::setw(7) << id << std::setw(24) << Utils::fit(all[i].getName(), 23)
                  << std::setw(8) << appointments.countForDoctor(id)
                  << std::setw(11) << appointments.countForDoctor(id, Appointment::SCHEDULED)
                  << std::setw(11) << appointments.countForDoctor(id, Appointment::COMPLETED)
                  << std::setw(10) << appointments.countForDoctor(id, Appointment::CANCELLED) << "\n";
    }
    Utils::printLine('-', 70);
}
