# Advanced Hospital Management System (C++ / OOP)

A menu-driven, file-based Hospital Management System written in standard C++17 as a
Semester 3 BCA (Data Science) Object-Oriented Programming group project.

> **Educational project.** All records are made-up sample data. This is *not* a real or
> production-ready medical system, and the simple password login is *not* real security.

---

## 1. Project Overview
The system lets hospital staff register patients and doctors, book and manage appointments,
keep visit (medical history) records, generate bills and payments, manage staff, and view a
dashboard. All data is stored in plain text files inside the `data/` folder, so it is still
there after the program closes.

## 2. Problem Statement
Small hospitals often keep patient, doctor, appointment and billing details in separate
paper registers. This causes duplicate entries, lost records, double-booked doctors and
billing mistakes. This project shows how an object-oriented C++ program can keep all of
these modules connected, validated and saved in one place.

## 3. Objectives
- Model a hospital with meaningful classes and relationships (inheritance, composition).
- Keep patients, doctors, appointments, records and bills consistent with each other.
- Validate every input and prevent duplicate IDs, double bookings and invalid amounts.
- Store data permanently using file handling and load it safely at start-up.
- Keep the code readable for a Semester 3 student.

## 4. Features and Modules
| Module | What it does |
|---|---|
| Patient Management | Register (auto unique ID), search by ID or name, list, update, delete with safety checks |
| Doctor Management | Register, search by ID / name / specialization, list, show available doctors, update (fee, shift, leave), remove |
| Appointment Management | Book, reschedule, cancel, complete; search by appointment ID or patient ID; blocks double bookings, past dates, doctors on leave and times outside the doctor's shift |
| Medical Records | Add visit records (diagnosis, prescription notes), view history by patient ID, search by record ID |
| Billing and Payments | Itemised bills, automatic consultation fee from the appointment, total calculation, Paid/Unpaid status, search by bill ID, unpaid list |
| Staff and Administration | Staff records (nurses, receptionists...), role-based menus (Administrator / Receptionist / Doctor), activity log |
| Dashboard and Reports | Totals, pending appointments, unpaid bills, outstanding and collected amounts, doctor workload |
| Data Persistence | Text files in `data/`, saved after every change, safe loading of missing / empty / invalid files |

### Safety rules built into the program
- A patient or doctor **cannot be deleted** while appointments, records or bills still refer to them (so nothing is silently corrupted). A doctor can be marked "On leave" instead.
- Only *completed* appointments can be billed, and an appointment can be billed only once.
- Duplicate patients (same name + contact), doctors and staff are rejected.
- Money values must be positive; quantities must be 1 to 100.
- On start-up the program warns about records that point to missing patients or doctors.

## 5. Technologies Used
- C++17 (standard library only: `iostream`, `fstream`, `vector`, `string`, `filesystem`, ...)
- `g++` compiler and `make` (optional)
- Plain text files for storage
- Git and GitHub for submission

## 6. OOP Concepts Demonstrated
| Concept | Where to find it |
|---|---|
| Classes and objects | `Patient`, `Doctor`, `Staff`, `Appointment`, `MedicalRecord`, `Bill`, `BillItem` |
| Data members and member functions | every class in `include/` and `src/` |
| Encapsulation / data hiding | private/protected members with getters and validated setters (e.g. `Person::setContact`) |
| Abstraction | `Person` (pure virtual functions) and `DataManager` are abstract classes |
| Constructors and destructors | all model classes; `ActivityLog::~ActivityLog()` writes a closing line and closes the file |
| public / private / protected | `Person` has protected members; derived classes use them, outside code cannot |
| Inheritance | `Person` -> `Patient`, `Doctor`, `Staff`; `DataManager` -> six manager classes; `HospitalException` -> `std::runtime_error` |
| Function overriding / runtime polymorphism | `display()`, `displaySummary()`, `toRecord()`, `getType()` overridden in derived classes; `HospitalSystem::showDirectory()` calls them through `Person*` pointers |
| Function overloading | `Doctor::isAvailable()` / `isAvailable(time)`, `Bill::addItem(...)` (3 versions), `HospitalSystem::searchPatient(int)` / `searchPatient(string)`, `AppointmentManager::countForDoctor(...)` (2 versions) |
| Constructor overloading | `Person()` / `Person(...)`, both `Appointment` and `Bill` constructors |
| Static members | `Patient::nextId`, `Patient::generateId()` (same pattern in every model); `Appointment::SCHEDULED` etc. |
| Scope resolution operator `::` | `Patient::generateId()`, `Appointment::COMPLETED`, `Utils::readInt()`, every `ClassName::function` definition |
| Pointers with classes | `findById()` returns `Patient*` / `Doctor*`; `std::vector<const Person*>` in the directory |
| Composition / aggregation | `HospitalSystem` owns all managers; each manager owns a `vector` of objects; `Bill` owns its `BillItem`s |
| File handling | `DataManager::load()` / `save()`, `ActivityLog` (`ofstream` append mode) |
| Exception handling | `HospitalException` thrown for every rule violation and caught in the menus |

**Outside Units 1 and 2 (explained in simple English):**
- `std::vector` - a resizable array from the standard library.
- `std::filesystem` (C++17) - only used to create the `data` folder and rename the temporary save file.
- `override` keyword - tells the compiler "this function replaces the base class version"; it catches typing mistakes.

## 7. Project Folder Structure
```
Hospital-Management-System/
├── README.md
├── Makefile
├── .gitignore
├── include/                 (header files: class declarations)
│   ├── Utils.h              input checking, text/date helpers, exception classes
│   ├── Person.h             abstract base class
│   ├── Patient.h  Doctor.h  Staff.h
│   ├── Appointment.h  MedicalRecord.h  Bill.h
│   ├── DataManager.h        abstract base class for file loading/saving
│   ├── PatientManager.h  DoctorManager.h  StaffManager.h
│   ├── AppointmentManager.h  RecordManager.h  BillManager.h
│   ├── ActivityLog.h
│   └── Hospital.h           HospitalSystem controller class
├── src/                     (implementation files)
│   ├── main.cpp
│   ├── Utils.cpp  Person.cpp  Patient.cpp  Doctor.cpp  Staff.cpp
│   ├── Appointment.cpp  MedicalRecord.cpp  Bill.cpp
│   ├── DataManager.cpp  *Manager.cpp  ActivityLog.cpp
│   ├── Hospital.cpp               core: start-up, login, main menu, directory
│   ├── HospitalPatients.cpp       patient module
│   ├── HospitalDoctors.cpp        doctor module
│   ├── HospitalStaff.cpp          staff module
│   ├── HospitalAppointments.cpp   appointment module
│   ├── HospitalRecords.cpp        medical records module
│   ├── HospitalBilling.cpp        billing module
│   └── HospitalReports.cpp        dashboard and reports
├── data/                    (sample data; the program reads and writes these files)
│   ├── patients.txt  doctors.txt  staff.txt
│   └── appointments.txt  records.txt  bills.txt
└── docs/
    ├── TESTING.md
    ├── WORK_DIVISION.md
    └── MOBILE_GITHUB_GUIDE.md
```

## 8. Compilation and Execution
Run all commands from the **project root folder** (the folder that contains `src/` and `data/`).

**Option A - with make (Linux / macOS / MinGW):**
```
make
./hospital
```
**Option B - one g++ command (works everywhere):**
```
g++ -std=c++17 -Iinclude src/*.cpp -o hospital
./hospital          (Windows: hospital.exe)
```
**Option C - compile each file separately (shows the build order):**
```
mkdir build
g++ -std=c++17 -Iinclude -c src/Utils.cpp -o build/Utils.o
...one line per .cpp file...
g++ build/*.o -o hospital
```
Source files can be compiled in any order, but all `.o` files must be linked together.
A C++17 compiler is required (g++ 9 or newer). To use another data folder:
`./hospital my_data_folder`

### Demo logins (learning demo only)
| Role | Password | Can do |
|---|---|---|
| Administrator | `admin123` | everything |
| Receptionist | `desk123` | patients, doctors (view), appointments, billing, dashboard |
| Doctor | `doctor123` | view patients/doctors/appointments, medical records, dashboard |

Passwords are written in the source code in plain text, so this is **not** real security.

## 9. How Data Files Work
- Each module has one text file in `data/`; each line is one record; fields are separated by `|`.
- Lines starting with `#` are comments. Bill items are stored as `description~price~quantity` separated by `;`.
- The characters `|`, `;` and `~` are removed from anything you type so they cannot break the format.
- Data is saved automatically after every change and again on exit. It is written to a `.tmp` file first and then renamed, so a crash cannot destroy the old file.
- Missing files are created automatically. Empty, invalid or duplicate lines are skipped and reported at start-up.
- IDs: patients start at 1001, doctors 2001, appointments 3001, bills 4001, records 5001, staff 6001. After loading, the next ID continues after the highest saved ID.
- `data/activity_log.txt` (ignored by git) records who did what.

## 10. Sample Program Workflow
1. Start the program, choose a role and enter its password.
2. **Patients -> Register new patient** -> receive ID 1009.
3. **Appointments -> Book new appointment** -> enter patient ID, pick an available doctor, date (`2026-12-10`) and time (`11:00`).
4. **Appointments -> Mark appointment as completed** -> optionally add the visit record, then generate the bill (consultation fee is added automatically; add lab tests or medicines).
5. **Billing -> Record payment** -> bill becomes *Paid*.
6. **Dashboard** -> see totals, pending appointments and unpaid bills.
7. Choose `0` to save and exit. Restart the program: everything is still there.

## 11. Limitations and Future Improvements
- Console only; no graphical interface.
- Passwords are stored in the source code; a real system needs hashing and user accounts.
- Appointment times are free-form `HH:MM` (no fixed 15/30-minute slots).
- Money uses `double`; a real billing system would store paise as integers.
- Text-file storage is simple but slow for very large data; a database (SQLite) would be better.
- Possible additions: search by date, discounts and tax on bills, printing receipts, patient allergy records, pharmacy and inventory module, unit tests.

## 12. How to Use the Application
Type the number of a menu option and press Enter. `0` always goes back. After each action the program waits for Enter. Dates are `YYYY-MM-DD`, times are 24-hour `HH:MM`, phone numbers are exactly 10 digits. Wrong input never crashes the program; it shows a message and asks again.

## 13. Credits
Developed as a Semester 3 BCA (Data Science) OOP group project.


