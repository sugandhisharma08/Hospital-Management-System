// Bill.h - a bill made of one or more items (COMPOSITION: a Bill owns its BillItem objects)
#ifndef BILL_H
#define BILL_H

#include <string>
#include <vector>

// One line on a bill, e.g. "Blood Test x 1 @ 300.00"
class BillItem {
private:
    std::string description;
    double unitPrice;
    int quantity;

public:
    BillItem(const std::string& desc, double price, int qty);
    std::string getDescription() const;
    double getUnitPrice() const;
    int getQuantity() const;
    double total() const;                    // unitPrice * quantity
};

class Bill {
private:
    int id;
    int patientId;
    int appointmentId;                       // 0 = not linked to an appointment
    std::string date;
    std::string status;                      // Paid / Unpaid
    std::vector<BillItem> items;             // composition
    static int nextId;

public:
    static const std::string PAID;
    static const std::string UNPAID;

    Bill(int billId, int pId, int appId, const std::string& billDate);
    Bill(int billId, int pId, int appId, const std::string& billDate, const std::string& billStatus);

    int getId() const;
    int getPatientId() const;
    int getAppointmentId() const;
    std::string getDate() const;
    std::string getStatus() const;
    bool isPaid() const;
    int getItemCount() const;
    double getTotal() const;

    // FUNCTION OVERLOADING: three ways to add an item
    void addItem(const BillItem& item);
    void addItem(const std::string& description, double price);                 // quantity 1
    void addItem(const std::string& description, double price, int quantity);

    void markPaid();

    void display() const;
    std::string toRecord() const;

    static Bill fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
};

#endif
