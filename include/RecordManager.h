// RecordManager.h - stores all medical records and saves/loads records.txt
#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <vector>
#include "DataManager.h"
#include "MedicalRecord.h"

class RecordManager : public DataManager {
private:
    std::vector<MedicalRecord> records;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit RecordManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    const MedicalRecord* findById(int id) const;
    const std::vector<MedicalRecord>& getAll() const;
    void addRecord(const MedicalRecord& record);
    std::vector<const MedicalRecord*> forPatient(int patientId) const;
    int countForPatient(int patientId) const;
    int countForDoctor(int doctorId) const;
    bool hasRecordForAppointment(int appointmentId) const;
    std::string moduleName() const override;
};

#endif
