// HospitalBilling.cpp - Billing and Payments module
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void HospitalSystem::billingMenu() {
    if (currentRole == ROLE_DOCTOR) {
        denyAccess();
        Utils::pause();
        return;
    }
    while (true) {
        Utils::printTitle("BILLING AND PAYMENTS");
        std::cout << "  1. Create new bill\n"
                  << "  2. Record payment (mark bill as Paid)\n"
                  << "  3. Display all bills\n"
                  << "  4. Search bill by ID\n"
                  << "  5. Bills of a patient\n"
                  << "  6. Show unpaid bills\n"
                  << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 6);
        try {
            if (choice == 0) return;
            else if (choice == 1) createBill();
            else if (choice == 2) payBill();
            else if (choice == 3) displayAllBills();
            else if (choice == 4) searchBillById();
            else if (choice == 5) billsForPatient();
            else if (choice == 6) showUnpaidBills();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::showBillList(const std::vector<const Bill*>& list) const {
    Utils::printLine('-', 80);
    std::cout << std::left << "  " << std::setw(7) << "Bill" << std::setw(22) << "Patient" << std::setw(12) << "Date"
              << std::setw(8) << "Items" << std::setw(16) << "Total (Rs.)" << std::setw(8) << "Status" << "\n";
    Utils::printLine('-', 80);
    for (std::size_t i = 0; i < list.size(); i++) {
        const Bill& b = *list[i];
        std::cout << "  " << std::setw(7) << b.getId() << std::setw(22) << Utils::fit(patientName(b.getPatientId()), 21)
                  << std::setw(12) << b.getDate() << std::setw(8) << b.getItemCount()
                  << std::setw(16) << Utils::toFixed2(b.getTotal()) << std::setw(8) << b.getStatus() << "\n";
    }
    Utils::printLine('-', 80);
    std::cout << "  Total: " << list.size() << " bill(s)\n";
}

void HospitalSystem::displayAllBills() const {
    if (bills.count() == 0) {
        std::cout << "\n  No bills have been generated yet.\n";
        return;
    }
    std::vector<const Bill*> list;
    for (std::size_t i = 0; i < bills.getAll().size(); i++) list.push_back(&bills.getAll()[i]);
    showBillList(list);
}

void HospitalSystem::searchBillById() const {
    int id = Utils::readInt("Bill ID: ", 1, 999999999);
    const Bill* bill = bills.findById(id);
    if (bill == nullptr) throw HospitalException("No bill found with ID " + std::to_string(id) + ".");
    std::cout << "\n  Patient name: " << patientName(bill->getPatientId()) << "\n";
    bill->display();
}

void HospitalSystem::billsForPatient() const {
    int pid = Utils::readInt("Patient ID: ", 1, 999999999);
    if (!patients.exists(pid)) throw HospitalException("No patient found with ID " + std::to_string(pid) + ".");
    std::vector<const Bill*> list = bills.forPatient(pid);
    if (list.empty()) {
        std::cout << "\n  This patient has no bills.\n";
        return;
    }
    showBillList(list);
}

void HospitalSystem::showUnpaidBills() const {
    std::vector<const Bill*> list = bills.unpaid();
    if (list.empty()) {
        std::cout << "\n  There are no unpaid bills.\n";
        return;
    }
    showBillList(list);
    std::cout << "  Outstanding amount: " << Utils::formatMoney(bills.totalOutstanding()) << "\n";
}

// Menu option: bill for a patient, optionally linked to a completed appointment
void HospitalSystem::createBill() {
    Utils::printTitle("CREATE BILL");
    int pid = Utils::readInt("Patient ID: ", 1, 999999999);
    if (!patients.exists(pid)) throw HospitalException("No patient found with ID " + std::to_string(pid) + ".");
    int aid = Utils::readInt("Completed appointment ID to bill (0 for none): ", 0, 999999999);
    if (aid > 0) {
        const Appointment* a = appointments.findById(aid);
        if (a == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(aid) + ".");
        if (a->getPatientId() != pid) throw HospitalException("That appointment belongs to a different patient.");
        if (a->getStatus() != Appointment::COMPLETED)
            throw HospitalException("Only completed appointments can be billed (this one is " + a->getStatus() + ").");
        if (bills.hasBillForAppointment(aid)) throw HospitalException("This appointment has already been billed.");
    }
    buildBill(pid, aid);
}

void HospitalSystem::createBillForAppointment(const Appointment& appointment) {
    if (bills.hasBillForAppointment(appointment.getId()))
        throw HospitalException("This appointment has already been billed.");
    buildBill(appointment.getPatientId(), appointment.getId());
}

// Collects the bill items, then creates the Bill object and stores it.
void HospitalSystem::buildBill(int patientId, int appointmentId) {
    std::vector<BillItem> items;
    if (appointmentId > 0) {
        const Appointment* a = appointments.findById(appointmentId);
        const Doctor* d = (a != nullptr) ? doctors.findById(a->getDoctorId()) : nullptr;
        if (d == nullptr) throw HospitalException("The doctor of this appointment no longer exists.");
        items.push_back(BillItem("Consultation Fee", d->getFee(), 1));
        std::cout << "  Added: Consultation Fee " << Utils::formatMoney(d->getFee()) << " (" << d->getName() << ")\n";
    }
    bool more = items.empty() ? true : Utils::readYesNo("  Add extra services (lab test, medicine, room...)? (y/n): ");
    while (more) {
        std::string description = Utils::readText("  Service description: ");
        double price = Utils::readMoney("  Unit price (Rs.): ", 0.01, 1000000);
        int quantity = Utils::readInt("  Quantity (1-100): ", 1, 100);
        items.push_back(BillItem(description, price, quantity));
        more = Utils::readYesNo("  Add another item? (y/n): ");
    }

    Bill bill(Bill::generateId(), patientId, appointmentId, Utils::todayDate());
    for (std::size_t i = 0; i < items.size(); i++) bill.addItem(items[i]);
    bills.addBill(bill);
    autoSave();
    activityLog.write("Created bill " + std::to_string(bill.getId()));
    std::cout << "\n  Bill created. Bill ID: " << bill.getId() << "\n";
    bill.display();
}

void HospitalSystem::payBill() {
    Utils::printTitle("RECORD PAYMENT");
    int id = Utils::readInt("Bill ID: ", 1, 999999999);
    Bill* bill = bills.findById(id);
    if (bill == nullptr) throw HospitalException("No bill found with ID " + std::to_string(id) + ".");
    if (bill->isPaid()) throw HospitalException("Bill " + std::to_string(id) + " is already paid.");
    bill->display();
    if (!Utils::readYesNo("  Confirm that " + Utils::formatMoney(bill->getTotal()) + " has been received? (y/n): ")) {
        std::cout << "  Nothing was changed.\n";
        return;
    }
    bill->markPaid();
    autoSave();
    activityLog.write("Payment received for bill " + std::to_string(id));
    std::cout << "\n  Payment recorded. Bill " << id << " is now Paid.\n";
}
