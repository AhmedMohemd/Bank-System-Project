#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Parser.h"
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include "DataSourceInterface.h"
using namespace std;
class FileManager : public DataSourceInterface {
public:
    static void addClient(const Client& client) {
        ofstream out("Clients.txt", ios::app);
        out << client.getId() << "," << client.getName() << "," << client.getPhone() << ","
            << client.getEmail() << "," << client.getPassword() << "," << client.getNationaid()
            << "," << client.getBalance() << endl;
        out.close();
    }
    static void addEmployee(const Employee& employee) {
        ofstream out("Employee.txt", ios::app);
        out << employee.getId() << "," << employee.getName() << "," << employee.getPhone() << ","
            << employee.getEmail() << "," << employee.getPassword() << "," << employee.getNationaid()
            << "," << employee.getSalary() << endl;
        out.close();
    }
    static void addAdmin(const Admin& admin) {
        ofstream out("Admin.txt", ios::app);
        out << admin.getId() << "," << admin.getName() << "," << admin.getPhone() << ","
            << admin.getEmail() << "," << admin.getPassword() << "," << admin.getNationaid() << endl;
        out.close();
    }
    static void getAllClients() {
        ifstream in("Clients.txt");
        string line;
        while (getline(in, line)) {
            if (!line.empty()) {
                Parser::parseToClient(line);
            }
        }
        in.close();
    }
    static void getAllEmployees() {
        ifstream in("Employee.txt");
        string line;
        while (getline(in, line)) {
            if (!line.empty()) {
                Parser::parseToEmployee(line);
            }
        }
        in.close();
    }
    static void getAllAdmins() {
        ifstream in("Admin.txt");
        string line;
        while (getline(in, line)) {
            if (!line.empty()) {
                Parser::parseToAdmin(line);
            }
        }
        in.close();
    }
    static void removeAllClients() {
        ofstream out("Clients.txt", ios::trunc);
        out.close();
    }
    static void removeAllEmployees() {
        ofstream out("Employee.txt", ios::trunc);
        out.close();
    }
    static void removeAllAdmins() {
        ofstream out("Admin.txt", ios::trunc);
        out.close();
    }
};
