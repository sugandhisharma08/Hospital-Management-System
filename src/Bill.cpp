// Bill.cpp - implementation of BillItem and Bill
#include "Bill.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ---------------- BillItem ----------------
BillItem::BillItem(const std::string& desc, double price, int qty)
    : description(""), unitPrice(0), quantity(1) {
    std::string d = Utils::clean(desc);
    if (d.empty()) throw HospitalException("Bill item description cannot be empty.");
    if (price <= 0 || price > 1000000) throw HospitalException("Item price must be between 0.01 and 1000000.");
    if (qty < 1 || qty > 100) throw HospitalException("Item quantity must be between 1 and 100.");
    description = d;
    unitPrice = price;
    quantity = qty;
}

std::string BillItem::getDescription() const { return description; }
double BillItem::getUnitPrice() const { return unitPrice; }
int BillItem::getQuantity() const { return quantity; }
double BillItem::total() const { return unitPrice * quantity; }

// ---------------- Bill ----------------
int Bill::nextId = 4001;
const std::string Bill::PAID = "Paid";
const std::string Bill::UNPAID = "Unpaid";

Bill::Bill(int billId, int pId, int appId, const std::string& billDate)
    : Bill(billId, pId, appId, billDate, "Unpaid") {}

Bill::Bill(int billId, int pId, int appId, const std::string& billDate, const std::string& billStatus)
    : id(billId), patientId(pId), appointmentId(appId), date(""), status("") {
    if (billId <= 0 || pId <= 0) throw HospitalException("Bill and patient IDs must be positive.");
    if (appId < 0) throw HospitalException("Appointment ID cannot be negative.");
    if (!Utils::isValidDate(billDate)) throw HospitalException("Invalid bill date.");
    if (billStatus != PAID && billStatus != UNPAID) throw HospitalException("Bill status must be Paid or Unpaid.");
    date = billDate;
    status = billStatus;
}

int Bill::getId() const { return id; }
int Bill::getPatientId() const { return patientId; }
int Bill::getAppointmentId() const { return appointmentId; }
std::string Bill::getDate() const { return date; }
std::string Bill::getStatus() const { return status; }
bool Bill::isPaid() const { return status == PAID; }
int Bill::getItemCount() const { return static_cast<int>(items.size()); }

double Bill::getTotal() const {
    double sum = 0;
    for (std::size_t i = 0; i < items.size(); i++) sum += items[i].total();
    return sum;
}

void Bill::addItem(const BillItem& item) {
    if (isPaid()) throw HospitalException("Cannot add items to a bill that is already paid.");
    items.push_back(item);
}

void Bill::addItem(const std::string& description, double price) {
    addItem(BillItem(description, price, 1));
}

void Bill::addItem(const std::string& description, double price, int quantity) {
    addItem(BillItem(description, price, quantity));
}

void Bill::markPaid() {
    if (isPaid()) throw HospitalException("This bill is already paid.");
    status = PAID;
}

void Bill::display() const {
    Utils::printLine('-', 66);
    std::cout << "  BILL " << id << "   |   Patient ID: " << patientId << "   |   Date: " << date << "\n"
              << "  Appointment: " << (appointmentId == 0 ? std::string("-") : std::to_string(appointmentId))
              << "   |   Status: " << status << "\n";
    Utils::printLine('-', 66);
    std::cout << std::left << std::setw(4) << "  #" << std::setw(28) << "Description"
              << std::setw(5) << "Qty" << std::setw(13) << "Unit Price" << "Total" << "\n";
    for (std::size_t i = 0; i < items.size(); i++) {
        std::cout << "  " << std::left << std::setw(2) << (i + 1) << std::setw(28) << Utils::fit(items[i].getDescription(), 27)
                  << std::setw(5) << items[i].getQuantity()
                  << std::setw(13) << Utils::toFixed2(items[i].getUnitPrice())
                  << Utils::toFixed2(items[i].total()) << "\n";
    }
    Utils::printLine('-', 66);
    std::cout << "  TOTAL AMOUNT: " << Utils::formatMoney(getTotal()) << "\n";
    Utils::printLine('-', 66);
}

// Item format inside the file: description~unitPrice~quantity;description~unitPrice~quantity
std::string Bill::toRecord() const {
    std::ostringstream out;
    out << id << '|' << patientId << '|' << appointmentId << '|' << date << '|' << status << '|';
    for (std::size_t i = 0; i < items.size(); i++) {
        if (i > 0) out << ';';
        out << items[i].getDescription() << '~' << Utils::toFixed2(items[i].getUnitPrice()) << '~' << items[i].getQuantity();
    }
    return out.str();
}

Bill Bill::fromRecord(const std::string& line) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 6) throw HospitalException("Bill record must have 6 fields.");
    Bill bill(Utils::toInt(f[0], "bill id"), Utils::toInt(f[1], "patient id"),
              Utils::toInt(f[2], "appointment id"), f[3], f[4]);
    std::vector<std::string> itemTexts = Utils::split(f[5], ';');
    for (std::size_t i = 0; i < itemTexts.size(); i++) {
        if (itemTexts[i].empty()) continue;
        std::vector<std::string> p = Utils::split(itemTexts[i], '~');
        if (p.size() != 3) throw HospitalException("Bad bill item format.");
        bill.items.push_back(BillItem(p[0], Utils::toDouble(p[1], "price"), Utils::toInt(p[2], "quantity")));
    }
    if (bill.items.empty()) throw HospitalException("A bill must contain at least one item.");
    return bill;
}

int Bill::generateId() { return nextId++; }

void Bill::syncNextId(int usedId) {
    if (usedId >= nextId) nextId = usedId + 1;
}
