// Doctor.h - a hospital doctor (derived from Person)
#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"

class Doctor : public Person {
private:
    std::string specialization;
    double fee;                  // consultation fee in rupees
    std::string shiftStart;      // HH:MM
    std::string shiftEnd;        // HH:MM
    bool onLeave;
    static int nextId;

public:
    Doctor(int did, const std::string& dname, int dage, char dgender,
           const std::string& dcontact, const std::string& daddress,
           const std::string& dspecialization, double dfee,
           const std::string& start, const std::string& end, bool leave);
    ~Doctor();

    std::string getSpecialization() const;
    double getFee() const;
    std::string getShiftStart() const;
    std::string getShiftEnd() const;
    bool isOnLeave() const;

    void setSpecialization(const std::string& value);
    void setFee(double value);
    void setShift(const std::string& start, const std::string& end);
    void setOnLeave(bool value);

    // FUNCTION OVERLOADING: same name, different parameters
    bool isAvailable() const;                          // not on leave?
    bool isAvailable(const std::string& time) const;   // free to see patients at this time?

    void display() const override;
    void displaySummary() const override;
    std::string toRecord() const override;
    std::string getType() const override;

    static Doctor fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
};

#endif
