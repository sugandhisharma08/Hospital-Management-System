// HospitalAppointments.cpp - Appointment Management module
#include "Hospital.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void HospitalSystem::appointmentMenu() {
    bool canEdit = (currentRole != ROLE_DOCTOR);       // doctors can only view appointments
    while (true) {
        Utils::printTitle("APPOINTMENT MANAGEMENT");
        std::cout << "  1. Display all appointments\n"
                  << "  2. Search appointment by ID\n"
                  << "  3. Search appointments by patient ID\n";
        if (canEdit) {
            std::cout << "  4. Book new appointment\n  5. Reschedule appointment\n"
                      << "  6. Cancel appointment\n  7. Mark appointment as completed\n";
        }
        std::cout << "  0. Back to main menu\n";
        int choice = Utils::readInt("Enter choice: ", 0, 7);
        try {
            if (choice == 0) return;
            else if (choice == 1) displayAllAppointments();
            else if (choice == 2) searchAppointmentById();
            else if (choice == 3) searchAppointmentsByPatient();
            else if (choice == 4 && canEdit) bookAppointment();
            else if (choice == 5 && canEdit) rescheduleAppointment();
            else if (choice == 6 && canEdit) cancelAppointment();
            else if (choice == 7 && canEdit) completeAppointment();
            else denyAccess();
        } catch (const HospitalException& e) {
            std::cout << "\n  Error: " << e.what() << "\n";
        }
        Utils::pause();
    }
}

void HospitalSystem::showAppointmentList(const std::vector<const Appointment*>& list) const {
    Utils::printLine('-', 100);
    std::cout << std::left << "  " << std::setw(6) << "ID" << std::setw(20) << "Patient" << std::setw(22) << "Doctor"
              << std::setw(12) << "Date" << std::setw(7) << "Time" << std::setw(11) << "Status" << "Reason\n";
    Utils::printLine('-', 100);
    for (std::size_t i = 0; i < list.size(); i++) {
        const Appointment& a = *list[i];
        std::cout << "  " << std::setw(6) << a.getId()
                  << std::setw(20) << Utils::fit(patientName(a.getPatientId()), 19)
                  << std::setw(22) << Utils::fit(doctorName(a.getDoctorId()), 21)
                  << std::setw(12) << a.getDate() << std::setw(7) << a.getTimeSlot()
                  << std::setw(11) << a.getStatus() << Utils::fit(a.getReason(), 30) << "\n";
    }
    Utils::printLine('-', 100);
    std::cout << "  Total: " << list.size() << " appointment(s)\n";
}

void HospitalSystem::displayAllAppointments() const {
    if (appointments.count() == 0) {
        std::cout << "\n  No appointments have been booked yet.\n";
        return;
    }
    std::vector<const Appointment*> list;
    for (std::size_t i = 0; i < appointments.getAll().size(); i++) list.push_back(&appointments.getAll()[i]);
    showAppointmentList(list);
}

void HospitalSystem::searchAppointmentById() const {
    int id = Utils::readInt("Enter appointment ID: ", 1, 999999999);
    const Appointment* a = appointments.findById(id);
    if (a == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(id) + ".");
    std::cout << "\n";
    a->display();
    std::cout << "  Patient name  : " << patientName(a->getPatientId()) << "\n"
              << "  Doctor name   : " << doctorName(a->getDoctorId()) << "\n";
}

void HospitalSystem::searchAppointmentsByPatient() const {
    int pid = Utils::readInt("Enter patient ID: ", 1, 999999999);
    if (!patients.exists(pid)) throw HospitalException("No patient found with ID " + std::to_string(pid) + ".");
    std::vector<const Appointment*> list = appointments.forPatient(pid);
    if (list.empty()) {
        std::cout << "\n  This patient has no appointments.\n";
        return;
    }
    showAppointmentList(list);
}

// The doctor must not be on leave and the time must fall inside the doctor's shift.
void HospitalSystem::checkDoctorTime(const Doctor& doctor, const std::string& time) const {
    if (doctor.isOnLeave()) throw HospitalException(doctor.getName() + " is on leave.");
    if (!doctor.isAvailable(time))
        throw HospitalException(doctor.getName() + " only sees patients between " + doctor.getShiftStart() +
                                " and " + doctor.getShiftEnd() + ".");
}

void HospitalSystem::bookAppointment() {
    Utils::printTitle("BOOK APPOINTMENT");
    int pid = Utils::readInt("Patient ID: ", 1, 999999999);
    const Patient* patient = patients.findById(pid);
    if (patient == nullptr) throw HospitalException("No patient found with ID " + std::to_string(pid) + ". Register the patient first.");
    std::cout << "  Patient: " << patient->getName() << "\n\n  Available doctors:\n";
    showAvailableDoctors();

    int did = Utils::readInt("Doctor ID: ", 1, 999999999);
    const Doctor* doctor = doctors.findById(did);
    if (doctor == nullptr) throw HospitalException("No doctor found with ID " + std::to_string(did) + ".");
    if (!doctor->isAvailable()) throw HospitalException(doctor->getName() + " is on leave.");

    std::string date = Utils::readDate("Appointment date (YYYY-MM-DD): ", true);
    std::string slot = Utils::readTime("Appointment time (HH:MM, 24 hour): ");
    if (date == Utils::todayDate() && slot < Utils::nowTime())
        throw HospitalException("That time has already passed today.");
    checkDoctorTime(*doctor, slot);
    std::string reason = Utils::readText("Reason for visit: ");
    appointments.checkSlotFree(did, pid, date, slot, 0);

    Appointment appointment(Appointment::generateId(), pid, did, date, slot, reason);
    appointments.addAppointment(appointment);
    autoSave();
    activityLog.write("Booked appointment " + std::to_string(appointment.getId()));
    std::cout << "\n  Appointment booked successfully. Appointment ID: " << appointment.getId() << "\n"
              << "  Consultation fee: " << Utils::formatMoney(doctor->getFee()) << "\n";
}

void HospitalSystem::rescheduleAppointment() {
    Utils::printTitle("RESCHEDULE APPOINTMENT");
    int id = Utils::readInt("Appointment ID: ", 1, 999999999);
    Appointment* appointment = appointments.findById(id);
    if (appointment == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(id) + ".");
    if (!appointment->isScheduled())
        throw HospitalException("Only scheduled appointments can be rescheduled (this one is " + appointment->getStatus() + ").");
    const Doctor* doctor = doctors.findById(appointment->getDoctorId());
    if (doctor == nullptr) throw HospitalException("The doctor of this appointment no longer exists.");

    std::cout << "  Current slot: " << appointment->getDate() << " at " << appointment->getTimeSlot() << "\n";
    std::string date = Utils::readDate("New date (YYYY-MM-DD): ", true);
    std::string slot = Utils::readTime("New time (HH:MM): ");
    if (date == Utils::todayDate() && slot < Utils::nowTime())
        throw HospitalException("That time has already passed today.");
    checkDoctorTime(*doctor, slot);
    appointments.checkSlotFree(appointment->getDoctorId(), appointment->getPatientId(), date, slot, id);

    appointment->reschedule(date, slot);
    autoSave();
    activityLog.write("Rescheduled appointment " + std::to_string(id));
    std::cout << "\n  Appointment rescheduled to " << date << " at " << slot << ".\n";
}

void HospitalSystem::cancelAppointment() {
    Utils::printTitle("CANCEL APPOINTMENT");
    int id = Utils::readInt("Appointment ID: ", 1, 999999999);
    Appointment* appointment = appointments.findById(id);
    if (appointment == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(id) + ".");
    if (!appointment->isScheduled())
        throw HospitalException("Only scheduled appointments can be cancelled (this one is " + appointment->getStatus() + ").");
    appointment->display();
    if (!Utils::readYesNo("\n  Really cancel this appointment? (y/n): ")) {
        std::cout << "  Nothing was changed.\n";
        return;
    }
    appointment->cancel();
    autoSave();
    activityLog.write("Cancelled appointment " + std::to_string(id));
    std::cout << "\n  Appointment cancelled.\n";
}

// Completing an appointment connects three modules: appointment -> medical record -> bill.
void HospitalSystem::completeAppointment() {
    Utils::printTitle("COMPLETE APPOINTMENT");
    int id = Utils::readInt("Appointment ID: ", 1, 999999999);
    Appointment* appointment = appointments.findById(id);
    if (appointment == nullptr) throw HospitalException("No appointment found with ID " + std::to_string(id) + ".");
    if (!appointment->isScheduled())
        throw HospitalException("Only scheduled appointments can be completed (this one is " + appointment->getStatus() + ").");
    appointment->complete();
    autoSave();
    activityLog.write("Completed appointment " + std::to_string(id));
    std::cout << "\n  Appointment " << id << " marked as Completed.\n";

    if (currentRole == ROLE_ADMIN && !records.hasRecordForAppointment(id)) {
        if (Utils::readYesNo("  Add the visit record (diagnosis/prescription) now? (y/n): "))
            addRecordForAppointment(*appointment);
    }
    if (!bills.hasBillForAppointment(id)) {
        if (Utils::readYesNo("  Generate the consultation bill now? (y/n): "))
            createBillForAppointment(*appointment);
    }
}
