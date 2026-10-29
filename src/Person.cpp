// Person.cpp - implementation of the Person base class
#include "Person.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <cctype>

Person::Person() : id(0), name(""), age(0), gender('O'), contact(""), address("") {}

Person::Person(int personId, const std::string& personName, int personAge,
               char personGender, const std::string& personContact,
               const std::string& personAddress)
    : id(personId), name(""), age(0), gender('O'), contact(""), address("") {
    if (personId <= 0) throw HospitalException("ID must be a positive number.");
    setName(personName);
    setAge(personAge);
    setGender(personGender);
    setContact(personContact);
    setAddress(personAddress);
}

Person::~Person() {}

int Person::getId() const { return id; }
std::string Person::getName() const { return name; }
int Person::getAge() const { return age; }
char Person::getGender() const { return gender; }
std::string Person::getContact() const { return contact; }
std::string Person::getAddress() const { return address; }

std::string Person::getGenderText() const {
    if (gender == 'M') return "Male";
    if (gender == 'F') return "Female";
    return "Other";
}

void Person::setName(const std::string& value) {
    std::string text = Utils::clean(value);
    if (text.empty()) throw HospitalException("Name cannot be empty.");
    name = text;
}

void Person::setAge(int value) {
    if (value < 0 || value > 120) throw HospitalException("Age must be between 0 and 120.");
    age = value;
}

void Person::setGender(char value) {
    char g = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
    if (g != 'M' && g != 'F' && g != 'O') throw HospitalException("Gender must be M, F or O.");
    gender = g;
}

void Person::setContact(const std::string& value) {
    if (!Utils::isValidPhone(value)) throw HospitalException("Contact number must contain exactly 10 digits.");
    contact = value;
}

void Person::setAddress(const std::string& value) {
    std::string text = Utils::clean(value);
    if (text.empty()) throw HospitalException("Address cannot be empty.");
    address = text;
}

std::string Person::basicRecord() const {
    std::ostringstream out;
    out << id << '|' << name << '|' << age << '|' << gender << '|' << contact << '|' << address;
    return out.str();
}

void Person::displayBasic() const {
    std::cout << "  ID            : " << id << "\n"
              << "  Name          : " << name << "\n"
              << "  Age           : " << age << "\n"
              << "  Gender        : " << getGenderText() << "\n"
              << "  Contact       : " << contact << "\n"
              << "  Address       : " << address << "\n";
}

void Person::displaySummary() const {
    std::cout << "  [" << getType() << "] " << id << " - " << name << " (" << contact << ")\n";
}
