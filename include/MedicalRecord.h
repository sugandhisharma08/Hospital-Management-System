// MedicalRecord.h - one visit entry in a patient's medical history (educational demo only)
#ifndef MEDICALRECORD_H
#define MEDICALRECORD_H

#include <string>

class MedicalRecord {
private:
    int id;
    int patientId;
    int doctorId;
    int appointmentId;           // 0 means "not linked to an appointment"
    std::string visitDate;
    std::string diagnosis;
    std::string prescription;
    std::string notes;
    static int nextId;

public:
    MedicalRecord(int recId, int pId, int dId, int appId, const std::string& date,
                  const std::string& diag, const std::string& prescr, const std::string& note);

    int getId() const;
    int getPatientId() const;
    int getDoctorId() const;
    int getAppointmentId() const;
    std::string getVisitDate() const;
    std::string getDiagnosis() const;

    void display() const;
    std::string toRecord() const;

    static MedicalRecord fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
};

#endif
