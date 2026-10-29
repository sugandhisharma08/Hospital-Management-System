// Person.h - abstract base class for Patient, Doctor and Staff
#ifndef PERSON_H
#define PERSON_H

#include <string>

// ABSTRACTION: Person has pure virtual functions, so you cannot create a plain Person object.
// ENCAPSULATION: data members are protected; outside code uses getters and validated setters.
class Person {
protected:                       // protected: visible to derived classes only
    int id;
    std::string name;
    int age;
    char gender;                 // 'M', 'F' or 'O'
    std::string contact;
    std::string address;

    std::string basicRecord() const;   // id|name|age|gender|contact|address
    void displayBasic() const;         // prints the common fields

public:
    Person();                                        // default constructor
    Person(int personId, const std::string& personName, int personAge,
           char personGender, const std::string& personContact,
           const std::string& personAddress);        // parameterised constructor (overloading)
    Person(const Person& other) = default;
    Person& operator=(const Person& other) = default;
    virtual ~Person();                               // virtual destructor (needed for polymorphism)

    // getters
    int getId() const;
    std::string getName() const;
    int getAge() const;
    char getGender() const;
    std::string getGenderText() const;
    std::string getContact() const;
    std::string getAddress() const;

    // validated setters (throw HospitalException for bad values)
    void setName(const std::string& value);
    void setAge(int value);
    void setGender(char value);
    void setContact(const std::string& value);
    void setAddress(const std::string& value);

    // Pure virtual functions: every derived class MUST override them (runtime polymorphism)
    virtual void display() const = 0;
    virtual std::string toRecord() const = 0;
    virtual std::string getType() const = 0;

    // Virtual with a default body: derived classes MAY override it
    virtual void displaySummary() const;
};

#endif
