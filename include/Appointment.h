// Appointment.h - one appointment between a patient and a doctor
#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

class Appointment {
private:
    int id;
    int patientId;               // links to Patient::getId()
    int doctorId;                // links to Doctor::getId()
    std::string date;            // YYYY-MM-DD
    std::string timeSlot;        // HH:MM
    std::string reason;
    std::string status;          // Scheduled / Completed / Cancelled
    static int nextId;

public:
    static const std::string SCHEDULED;
    static const std::string COMPLETED;
    static const std::string CANCELLED;

    // CONSTRUCTOR OVERLOADING: the short one creates a "Scheduled" appointment
    Appointment(int appId, int pId, int dId, const std::string& appDate,
                const std::string& appTime, const std::string& appReason);
    Appointment(int appId, int pId, int dId, const std::string& appDate,
                const std::string& appTime, const std::string& appReason,
                const std::string& appStatus);

    int getId() const;
    int getPatientId() const;
    int getDoctorId() const;
    std::string getDate() const;
    std::string getTimeSlot() const;
    std::string getReason() const;
    std::string getStatus() const;
    bool isScheduled() const;

    void reschedule(const std::string& newDate, const std::string& newTime);
    void cancel();
    void complete();

    void display() const;
    std::string toRecord() const;

    static Appointment fromRecord(const std::string& line);
    static int generateId();
    static void syncNextId(int usedId);
};

#endif
