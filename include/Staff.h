// Staff.h - non-doctor hospital staff such as nurses and receptionists (derived from Person)
#ifndef STAFF_H
#define STAFF_H

#include "Person.h"

class Staff : public Person {
private:
    std::string role;            // e.g. Nurse, Receptionist
    std::string department;
    double salary;
    static int nextId;

public:
    Staff(int sid, const std::string& sname, int sage, char sgender,
          const std::string& scontact, const std::string& saddress,
          const std::string& srole, const std::string& sdepartment, double ssalary);
    ~Staff();

    std::string getRole() const;
    std::string getDepartment() const;
    double getSalary() const;
    void setRole(const std::string& value);
    void setDepartment(const std::string& value);
    void setSalary(double value);

    void display() const override;
    void displaySummary() const override;
    std::string toRecord() const override;
    std::string getType() const override;

    static Staff fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
};

#endif
