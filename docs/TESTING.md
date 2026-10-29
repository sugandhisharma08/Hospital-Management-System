# Testing Guide

## How the project was verified
- Compiled with `g++ -std=c++17 -Wall -Wextra` (g++ 13): **zero warnings, zero errors**.
- Compiled again with AddressSanitizer + UBSan and run through a long scripted session
  (about 100 menu actions across all modules): **no memory or undefined-behaviour errors**.
- Re-started the program to confirm saved data is reloaded correctly.
- Tested damaged data files (bad lines, duplicate IDs, empty file, orphan appointment, missing folder).
- Tested the three login roles and 3 wrong passwords.

You should repeat a few of these yourself before the viva (see the tables below).
Always start from the original sample data (`git checkout data` or re-download the data folder) so IDs match.

## Test cases (Administrator, sample data loaded)
Sample data: 8 patients (1001-1008), 5 doctors (2001-2005), 4 staff, 7 appointments, 3 records, 3 bills.

### Patients
| # | Action | Expected result |
|---|---|---|
| P1 | Register "Test Patient", age `abc`, then `-5`, then `35` | Age prompt repeats with an error twice, then accepts 35 |
| P2 | Phone `123` | "Phone number must contain exactly 10 digits." and asks again |
| P3 | Register the same name + same contact again | Error: already registered (ID shown) |
| P4 | Search by ID `9999` | "No patient found with ID 9999." |
| P5 | Search by name `aarav` | Table shows Aarav Mehta (case-insensitive, partial match) |
| P6 | Update patient, change age | Saved; list shows new age |
| P7 | Delete patient 1003 | Blocked: "linked data exists (2 appointments, 0 records, 0 bills)" |
| P8 | Delete the newly registered patient (no links) and confirm `y` | Patient deleted |

### Doctors
| # | Action | Expected result |
|---|---|---|
| D1 | Search `cardio` | Dr. Neha Kulkarni |
| D2 | Show available doctors | 4 doctors (2004 is on leave) |
| D3 | Register doctor with fee `-5` then `450`, shift 10:00-09:00 | Fee re-prompted; then error "start time must be earlier than end time" |
| D4 | Remove doctor 2002 | Blocked (has appointments and records) |
| D5 | Toggle leave for 2003 twice | ON LEAVE then AVAILABLE |

### Appointments
| # | Action | Expected result |
|---|---|---|
| A1 | Book patient 1008, doctor 2003, 2026-12-10, 11:00 | Booked, ID 3008, fee shown |
| A2 | Book patient 1007, same doctor/date/time | Error: doctor already has an appointment |
| A3 | Book patient 1008 with doctor 2002 at same date/time | Error: patient already has an appointment |
| A4 | Book with doctor 2004 | Error: doctor is on leave |
| A5 | Book doctor 2005 at 15:00 (shift ends 13:00) | Error: only sees patients between 09:00 and 13:00 |
| A6 | Date `2020-01-01` | "The date cannot be in the past", asks again |
| A7 | Date `2026-02-30`, time `25:00` | Invalid date / invalid time messages, asks again |
| A8 | Unknown patient ID | Error: register the patient first |
| A9 | Reschedule 3003 to 2026-12-12 11:00 | Rescheduled |
| A10 | Reschedule another appointment of doctor 2002 into the same slot | Error: doctor already has an appointment |
| A11 | Reschedule a Completed appointment (3001) | Error: only scheduled appointments |
| A12 | Cancel 3006 (`y`), then cancel again | Cancelled; second attempt is refused |
| A13 | Complete 3003, add record, generate bill with an extra item | Status Completed, record 5004 created, bill 4004 total = fee + extras |

### Medical records
| # | Action | Expected result |
|---|---|---|
| M1 | Add record for patient 1001 with appointment 3002 (belongs to another patient) | Error: different patient or doctor |
| M2 | Visit date 2099-01-01 | Error: cannot have a future date |
| M3 | View history of patient 1001 | Shows record 5001 with doctor name |
| M4 | View history of patient 1008 before adding any | "No medical records found" |

### Billing
| # | Action | Expected result |
|---|---|---|
| B1 | Create bill: price `0`, then `1500`; quantity `0`, then `2`; add Medicine `250.50` x 1 | Price/quantity re-prompted; total = 3000 + 250.50 = **Rs. 3250.50** |
| B2 | Bill for a Scheduled appointment (3004) | Error: only completed appointments can be billed |
| B3 | Bill again for already billed appointment 3001 | Error: already billed |
| B4 | Pay bill 4002 (`y`) then pay again | Paid; second attempt: "already paid" |
| B5 | Pay bill and answer `n` | "Nothing was changed" |
| B6 | Search bill `9999` | "No bill found" |

### Roles, dashboard, persistence
| # | Action | Expected result |
|---|---|---|
| R1 | Login Receptionist (`desk123`) | Menu has no Staff, Records or Directory options |
| R2 | Login Doctor (`doctor123`), open Patients | No register/update/delete options; no Billing in main menu |
| R3 | Three wrong passwords | "Too many wrong passwords. Exiting." |
| R4 | Dashboard (original data) | 8 patients, 5 doctors, 4 staff, 7 appointments (3 scheduled, 3 completed, 1 cancelled), 3 records, 3 bills, 1 unpaid = Rs. 800.00 outstanding, Rs. 2300.00 collected |
| R5 | Make changes, choose `0`, restart | All changes still present |
| R6 | Put a garbage line and a duplicate ID in `data/patients.txt`, start | Start-up warning "invalid or duplicate line(s) skipped"; program still works |
| R7 | Delete the whole `data` folder, start, register a patient | Folder and files are re-created automatically |

## Sample output (bill from case A13)
```
  BILL 4004   |   Patient ID: 1003   |   Date: 2026-10-29
  Appointment: 3003   |   Status: Unpaid
  # Description                 Qty  Unit Price   Total
  1 Consultation Fee            1    500.00       500.00
  2 Blood sugar test            2    150.00       300.00
  TOTAL AMOUNT: Rs. 800.00
```

## Sample dashboard (original data)
```
  Registered patients   : 8
  Registered doctors    : 5
  Total appointments    : 7
     Scheduled (pending): 3
  Total bills           : 3
     Unpaid bills       : 1
     Outstanding amount : Rs. 800.00
     Amount collected   : Rs. 2300.00
```
