// Patient.h - a hospital patient (derived from Person)
#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"

class Patient : public Person {          // INHERITANCE: Patient "is a" Person
private:
    std::string bloodGroup;
    std::string emergencyContact;
    std::string registeredDate;
    static int nextId;                   // STATIC member: shared by all patients (ID generator)

public:
    Patient(int pid, const std::string& pname, int page, char pgender,
            const std::string& pcontact, const std::string& paddress,
            const std::string& blood, const std::string& emergency,
            const std::string& regDate);
    ~Patient();

    std::string getBloodGroup() const;
    std::string getEmergencyContact() const;
    std::string getRegisteredDate() const;
    void setBloodGroup(const std::string& value);
    void setEmergencyContact(const std::string& value);

    // Overridden virtual functions (runtime polymorphism)
    void display() const override;
    void displaySummary() const override;
    std::string toRecord() const override;
    std::string getType() const override;

    // Static functions belong to the class, not to one object
    static Patient fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
    static std::string normalizeBloodGroup(const std::string& text);   // "" if invalid
};

#endif
