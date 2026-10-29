// DoctorManager.h - stores all doctors and saves/loads doctors.txt
#ifndef DOCTORMANAGER_H
#define DOCTORMANAGER_H

#include <vector>
#include "DataManager.h"
#include "Doctor.h"

class DoctorManager : public DataManager {
private:
    std::vector<Doctor> doctors;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit DoctorManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    Doctor* findById(int id);
    const Doctor* findById(int id) const;
    std::vector<const Doctor*> searchByText(const std::string& text) const;   // name or specialization
    std::vector<const Doctor*> getAvailable() const;                          // doctors not on leave
    const std::vector<Doctor>& getAll() const;
    void addDoctor(const Doctor& doctor);
    void removeDoctor(int id);
    std::string moduleName() const override;
};

#endif
