# Work Division for Two Students

Fill in the names yourselves. Both students must understand the whole project.

## Student 1 - Core design, People modules and Persistence
Files owned:
- `Utils.h/.cpp`, `Person.*`, `Patient.*`, `Doctor.*`, `Staff.*`
- `DataManager.*`, `PatientManager.*`, `DoctorManager.*`, `StaffManager.*`
- `Hospital.h`, `Hospital.cpp`, `main.cpp`
- `HospitalPatients.cpp`, `HospitalDoctors.cpp`, `HospitalStaff.cpp`
- `data/patients.txt`, `data/doctors.txt`, `data/staff.txt`

Talking points: inheritance (`Person` -> `Patient/Doctor/Staff`), abstract class, virtual functions and
polymorphism (`showDirectory`), encapsulation and validated setters, static ID generators,
file loading/saving in `DataManager`, login and roles.

## Student 2 - Transaction modules
Files owned:
- `Appointment.*`, `MedicalRecord.*`, `Bill.*` (with `BillItem`)
- `AppointmentManager.*`, `RecordManager.*`, `BillManager.*`, `ActivityLog.*`
- `HospitalAppointments.cpp`, `HospitalRecords.cpp`, `HospitalBilling.cpp`, `HospitalReports.cpp`
- `data/appointments.txt`, `data/records.txt`, `data/bills.txt`

Talking points: composition (`Bill` owns `BillItem`s), function overloading (`addItem`, `countForDoctor`),
constructor overloading and delegation (`Appointment`), double-booking check, how completing an
appointment links to records and billing, destructor in `ActivityLog`, dashboard calculations.

## Shared by both
- `README.md`, `docs/`, testing, GitHub upload. Each student makes commits under their own name if possible.

## How the modules connect
```
 main.cpp -> HospitalSystem (Hospital.cpp)
               |-- owns PatientManager, DoctorManager, StaffManager      (Student 1 data)
               |-- owns AppointmentManager, RecordManager, BillManager   (Student 2 data)
               '-- menus are split into HospitalXxx.cpp files, one per module
 Links by ID:  Appointment -> patientId + doctorId
               MedicalRecord -> patientId + doctorId + appointmentId
               Bill -> patientId + appointmentId (fee comes from the Doctor)
 Delete rules: Hospital*.cpp checks the other managers before deleting a patient or doctor.
```

## Suggested integration and testing order
1. Student 1 builds and tests Patients and Doctors (cases P1-P8, D1-D5).
2. Student 2 builds and tests Appointments (A1-A13) using the sample patients and doctors.
3. Add Records (M1-M4) then Billing (B1-B6).
4. Run the role tests (R1-R7) together, then explain each other's modules to each other.
5. Compile everything with `make` and run the full test table in `docs/TESTING.md`.
