// Hospital.h - HospitalSystem is the "controller" class of the project.
// COMPOSITION: it owns one manager object for every module.
// The member functions are split over several .cpp files, one file per module,
// so that two students can work on different files without conflicts.
#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include "PatientManager.h"
#include "DoctorManager.h"
#include "StaffManager.h"
#include "AppointmentManager.h"
#include "RecordManager.h"
#include "BillManager.h"
#include "ActivityLog.h"

enum Role { ROLE_ADMIN = 1, ROLE_RECEPTIONIST = 2, ROLE_DOCTOR = 3 };

class HospitalSystem {
private:
    enum MenuAction {
        ACT_PATIENTS = 1, ACT_DOCTORS, ACT_STAFF, ACT_APPOINTMENTS, ACT_RECORDS,
        ACT_BILLING, ACT_REPORTS, ACT_DIRECTORY, ACT_ACTIVITY_LOG, ACT_SAVE
    };

    std::string dataDir;
    PatientManager patients;
    DoctorManager doctors;
    StaffManager staff;
    AppointmentManager appointments;
    RecordManager records;
    BillManager bills;
    ActivityLog activityLog;
    Role currentRole;

    // ---------- core (Hospital.cpp) ----------
    void loadAll();
    bool saveAll();
    void autoSave();
    void verifyDataIntegrity() const;
    bool login();
    std::string expectedPassword() const;
    std::string roleName() const;
    void denyAccess() const;
    std::vector<int> menuActionsForRole() const;
    std::string actionLabel(int action) const;
    void handleAction(int action);
    void showDirectory() const;
    void showActivityLog() const;
    std::string patientName(int id) const;
    std::string doctorName(int id) const;

    // ---------- patients (HospitalPatients.cpp) ----------
    void patientMenu();
    void registerPatient();
    void searchPatient(int id) const;                       // overloaded: search by ID
    void searchPatient(const std::string& name) const;      // overloaded: search by name
    void showPatientList(const std::vector<const Patient*>& list) const;
    void displayAllPatients() const;
    void updatePatient();
    void deletePatient();

    // ---------- doctors (HospitalDoctors.cpp) ----------
    void doctorMenu();
    void registerDoctor();
    void searchDoctors() const;
    void showDoctorList(const std::vector<const Doctor*>& list) const;
    void displayAllDoctors() const;
    void showAvailableDoctors() const;
    void updateDoctor();
    void deleteDoctor();

    // ---------- staff (HospitalStaff.cpp) ----------
    void staffMenu();
    void registerStaff();
    void searchStaff() const;
    void showStaffList(const std::vector<const Staff*>& list) const;
    void displayAllStaff() const;
    void updateStaff();
    void deleteStaff();

    // ---------- appointments (HospitalAppointments.cpp) ----------
    void appointmentMenu();
    void showAppointmentList(const std::vector<const Appointment*>& list) const;
    void displayAllAppointments() const;
    void searchAppointmentById() const;
    void searchAppointmentsByPatient() const;
    void bookAppointment();
    void rescheduleAppointment();
    void cancelAppointment();
    void completeAppointment();
    void checkDoctorTime(const Doctor& doctor, const std::string& time) const;

    // ---------- medical records (HospitalRecords.cpp) ----------
    void recordsMenu();
    void addMedicalRecord();
    void addRecordForAppointment(const Appointment& appointment);
    void viewRecordsByPatient() const;
    void viewAllRecords() const;
    void searchRecordById() const;

    // ---------- billing (HospitalBilling.cpp) ----------
    void billingMenu();
    void showBillList(const std::vector<const Bill*>& list) const;
    void displayAllBills() const;
    void searchBillById() const;
    void billsForPatient() const;
    void showUnpaidBills() const;
    void createBill();
    void createBillForAppointment(const Appointment& appointment);
    void buildBill(int patientId, int appointmentId);
    void payBill();

    // ---------- reports (HospitalReports.cpp) ----------
    void reportsMenu();
    void showDashboard() const;
    void showPendingAppointments() const;
    void showDoctorWorkload() const;

public:
    explicit HospitalSystem(const std::string& dataDirectory);
    ~HospitalSystem();
    void run();
};

#endif
