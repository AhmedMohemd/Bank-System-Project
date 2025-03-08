#pragma once
#include <iostream>
#include <string>
#include "Validation.h"
using namespace std;
class Person {
protected:
    string name, password, email, phone, nationaid;
    int id;
public:
    Person() {
        id = 0;
    }
    Person(const string& name, int id, const string& phone, const string& email, const string& password, const string& nationaid) {
        setName(name);
        setId(id);
        setPhone(phone);
        setEmail(email);
        setPassword(password);
        setNationaid(nationaid);
    }
    void setName(const string& name) {
        if (Validation::checkName(name))
            this->name = name;
    }
    void setId(int id) {
        if (Validation::checkID(id))
            this->id = id;
    }
    void setPhone(const string& phone) {
        if (Validation::checkPhone(phone))
            this->phone = phone;
    }
    void setEmail(const string& email) {
        if (Validation::checkEmail(email))
            this->email = email;
    }
    void setPassword(const string& password) {
        if (Validation::checkPassword(password))
            this->password = password;
    }
    void setNationaid(const string& nationaid) {
        if (Validation::checkNID(nationaid))
            this->nationaid = nationaid;
    }
    string getName() const {
        return this->name;
    }
    int getId() const {
        return this->id;
    }
    string getPhone() const {
        return this->phone;
    }
    string getEmail() const {
        return this->email;
    }
    string getPassword() const {
        return this->password;
    }
    string getNationaid() const {
        return this->nationaid;
    }
    virtual void displayInfo() const = 0;
    virtual ~Person() {
    }
};
