// AppointmentManager.h - stores all appointments and saves/loads appointments.txt
#ifndef APPOINTMENTMANAGER_H
#define APPOINTMENTMANAGER_H

#include <vector>
#include "DataManager.h"
#include "Appointment.h"

class AppointmentManager : public DataManager {
private:
    std::vector<Appointment> appointments;

protected:
    bool parseLine(const std::string& line) override;
    void writeRecords(std::ostream& out) const override;

public:
    explicit AppointmentManager(const std::string& path);

    int count() const;
    bool exists(int id) const;
    Appointment* findById(int id);
    const Appointment* findById(int id) const;
    const std::vector<Appointment>& getAll() const;
    void addAppointment(const Appointment& appointment);

    // Throws a HospitalException if the doctor or the patient already has a SCHEDULED
    // appointment at the same date and time (ignoreId lets us skip the appointment being moved).
    void checkSlotFree(int doctorId, int patientId, const std::string& date,
                       const std::string& time, int ignoreId) const;

    std::vector<const Appointment*> forPatient(int patientId) const;
    std::vector<const Appointment*> withStatus(const std::string& status) const;
    int countForPatient(int patientId) const;
    int countForDoctor(int doctorId) const;
    int countForDoctor(int doctorId, const std::string& status) const;    // overloaded
    int countByStatus(const std::string& status) const;
    std::string moduleName() const override;
};

#endif
