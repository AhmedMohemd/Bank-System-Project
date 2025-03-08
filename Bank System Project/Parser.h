#pragma once
#include <vector>
#include <sstream>
#include <stdexcept>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
using namespace std;
class Parser {
public:
    static vector<string> split(const string& line, char delimiter = ',') {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
    static void parseToClient(const string& line) {
        vector<string> tokens = split(line);
        if (tokens.size() == 7) {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            string phone = tokens[2];
            string email = tokens[3];
            string password = tokens[4];
            string nationaid = tokens[5];
            double balance = stod(tokens[6]);
            allClients.push_back(Client(name, id, phone, email, password, nationaid, balance));
        }
    }
    static void parseToEmployee(const string& line) {
        vector<string> tokens = split(line);
        if (tokens.size() == 7) {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            string phone = tokens[2];
            string email = tokens[3];
            string password = tokens[4];
            string nationaid = tokens[5];
            double salary = stod(tokens[6]);
            allEmployees.push_back(Employee(name, id, password, phone, email, nationaid, salary));
        }
    }
    static void parseToAdmin(const string& line) {
        vector<string> tokens = split(line);
        if (tokens.size() == 6) {
            int id = stoi(tokens[0]);
            string name = tokens[1];
            string phone = tokens[2];
            string email = tokens[3];
            string password = tokens[4];
            string nationaid = tokens[5];
            allAdmins.push_back(Admin(name, id, password, phone, email, nationaid));
        }
    }
};
