// BillManager.h - stores all bills and saves/loads bills.txt
#ifndef BILLMANAGER_H
#define BILLMANAGER_H

#include <vector>
#include "DataManager.h"
#include "Bill.h"

class BillManager : public DataManager {
private:
    std::vector<Bill> bills;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit BillManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    Bill* findById(int id);
    const Bill* findById(int id) const;
    const std::vector<Bill>& getAll() const;
    void addBill(const Bill& bill);           // a bill must contain at least one item
    std::vector<const Bill*> forPatient(int patientId) const;
    std::vector<const Bill*> unpaid() const;
    int countForPatient(int patientId) const;
    int countUnpaid() const;
    double totalOutstanding() const;          // sum of all unpaid bills
    double totalCollected() const;            // sum of all paid bills
    bool hasBillForAppointment(int appointmentId) const;
    std::string moduleName() const override;
};

#endif
