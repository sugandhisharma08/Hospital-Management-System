// PatientManager.h - stores all patients and saves/loads patients.txt
#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <vector>
#include "DataManager.h"
#include "Patient.h"

class PatientManager : public DataManager {
private:
    std::vector<Patient> patients;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit PatientManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    Patient* findById(int id);                    // returns nullptr if not found
    const Patient* findById(int id) const;        // overload for const objects
    std::vector<const Patient*> searchByName(const std::string& text) const;
    const std::vector<Patient>& getAll() const;
    void addPatient(const Patient& patient);      // throws if the ID already exists
    void removePatient(int id);                   // throws if the ID does not exist
    std::string moduleName() const override;
};

#endif
