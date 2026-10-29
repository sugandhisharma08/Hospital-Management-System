// BillManager.cpp
#include "BillManager.h"
#include "Utils.h"

BillManager::BillManager(const std::string& path) : DataManager(path) {}

bool BillManager::parseLine(const std::string& line) {
    Bill b = Bill::fromRecord(line);
    if (exists(b.getId())) return false;
    bills.push_back(b);
    Bill::syncNextId(b.getId());
    return true;
}

void BillManager::writeRecords(std::ostream& out) const {
    for (std::size_t i = 0; i < bills.size(); i++) out << bills[i].toRecord() << "\n";
}

int BillManager::count() const { return static_cast<int>(bills.size()); }

bool BillManager::exists(int id) const { return findById(id) != nullptr; }

Bill* BillManager::findById(int id) {
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].getId() == id) return &bills[i];
    return nullptr;
}

const Bill* BillManager::findById(int id) const {
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].getId() == id) return &bills[i];
    return nullptr;
}

const std::vector<Bill>& BillManager::getAll() const { return bills; }

void BillManager::addBill(const Bill& bill) {
    if (exists(bill.getId())) throw HospitalException("Bill ID already exists.");
    if (bill.getItemCount() == 0) throw HospitalException("A bill must contain at least one item.");
    if (bill.getTotal() <= 0) throw HospitalException("Bill total must be greater than zero.");
    bills.push_back(bill);
    Bill::syncNextId(bill.getId());
}

std::vector<const Bill*> BillManager::forPatient(int patientId) const {
    std::vector<const Bill*> result;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].getPatientId() == patientId) result.push_back(&bills[i]);
    return result;
}

std::vector<const Bill*> BillManager::unpaid() const {
    std::vector<const Bill*> result;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (!bills[i].isPaid()) result.push_back(&bills[i]);
    return result;
}

int BillManager::countForPatient(int patientId) const {
    int total = 0;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].getPatientId() == patientId) total++;
    return total;
}

int BillManager::countUnpaid() const {
    int total = 0;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (!bills[i].isPaid()) total++;
    return total;
}

double BillManager::totalOutstanding() const {
    double sum = 0;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (!bills[i].isPaid()) sum += bills[i].getTotal();
    return sum;
}

double BillManager::totalCollected() const {
    double sum = 0;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].isPaid()) sum += bills[i].getTotal();
    return sum;
}

bool BillManager::hasBillForAppointment(int appointmentId) const {
    if (appointmentId == 0) return false;
    for (std::size_t i = 0; i < bills.size(); i++)
        if (bills[i].getAppointmentId() == appointmentId) return true;
    return false;
}

std::string BillManager::moduleName() const { return "Bills"; }
