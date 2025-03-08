#pragma once
#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>
using namespace std;
class Validation {
public:
    static bool checkName(const string& name) {
        if (name.size() < 5 || name.size() > 32) {
            throw invalid_argument("ERROR: Name must be between 5 to 32 characters");
        }
        if (!regex_match(name, regex("^[a-zA-Z ]+$"))) {
            throw invalid_argument("ERROR: Name must contain only alphabetic characters");
        }
        return true;
    }
    static bool checkPassword(const string& password) {
        if (password.size() < 8 || password.size() > 20) {
            throw invalid_argument("ERROR: Password must be between 8 to 20 characters");
        }
        return true;
    }
    static bool checkID(int id) {
        if (id <= 0) {
            throw invalid_argument("ERROR: ID must be a positive number");
        }
        return true;
    }
    static bool checkBalance(double balance, double minimumBalance = 50) {
        if (balance < minimumBalance) {
            throw runtime_error("ERROR: Balance must be greater than or equal to " + to_string(minimumBalance));
        }
        return true;
    }
    static bool checkAmount(double amount) {
        if (amount <= 0) {
            throw invalid_argument("ERROR: Amount must be a positive number");
        }
        return true;
    }
    static bool checkEmail(const string& email) {
        if (!regex_match(email, regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})"))) {
            throw invalid_argument("ERROR: Invalid email format");
        }
        return true;
    }
    static bool checkPhone(const string& phone) {
        if (!regex_match(phone, regex("^01[0-9]{9}$"))) {
            throw invalid_argument("ERROR: Invalid phone number format");
        }
        return true;
    }
    static bool checkNID(const string& nid) {
        if (nid.size() != 14 || !regex_match(nid, regex("^[0-9]+$"))) {
            throw invalid_argument("ERROR: National ID must be 14 digits");
        }
        return true;
    }
    static bool checkSalary(double salary) {
        if (salary <= 0) {
            throw invalid_argument("ERROR: Salary must be greater than 0");
        }
        return true;
    }
};
