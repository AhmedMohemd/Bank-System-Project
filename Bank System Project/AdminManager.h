#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include "Admin.h"
#include "Employee.h"
#include "Client.h"
#include "FileManager.h"
#include "FileHelper.h"
using namespace std;
class AdminManager {
public:
    static void printAdminMenu() {
        cout << "1:Add Client" << endl;
        cout << "2:Search for cleint" << endl;
        cout << "3:Edit cleint" << endl;
        cout << "4:Display client info" << endl;
        cout << "5:List All cleints" << endl;
        cout << "6:Remove Cllent" << endl;
        cout << "7:Review Client Loan" << endl;
        cout << "8:Add Employee" << endl;
        cout << "9:Search for Employee" << endl;
        cout << "10:Edit Employee" << endl;
        cout << "11:Display Employee info" << endl;
        cout << "12:List All Employees" << endl;
        cout << "13:Remove Employee" << endl;
        cout << "14:Logout (Return to Main Menu)" << endl;
    }
    static Admin* login() {
        cout << "\033[2J\033[1;1H";
        int id;
        string inputPassword;
        int idAttempts = 3;
        while (idAttempts > 0) {
            cout << "Enter your Admin ID: ";
            cin >> id;
            bool idExists = false;
            for (const auto& admin : allAdmins) {
                if (admin.getId() == id) {
                    idExists = true;
                    break;
                }
            }
            if (!idExists) {
                idAttempts--;
                cout << "ERROR: ID not found! Please enter a valid Admin ID.\n";
                if (idAttempts > 0)
                    cout << "Attempts left: " << idAttempts << endl;
                else {
                    cout << "Too many failed attempts. Returning to main menu...\n";
                    return nullptr;
                }
            }
            else {
                break;
            }
        }
        int attempts = 3;
        while (attempts > 0) {
            cout << "Enter Password: ";
            cin >> inputPassword;
            for (auto& admin : allAdmins) {
                if (admin.getId() == id && admin.getPassword() == inputPassword) {
                    cout << "Login Successful" << endl;
                    return new Admin(admin);
                }
            }
            attempts--;
            if (attempts > 0)
                cout << "Invalid credentials ttempts left: " << attempts << endl;
        }
        cout << "Too many failed attempts please wait 1 minute" << endl;
        this_thread::sleep_for(chrono::minutes(1));
        return nullptr;
    }
    static Admin* registerAdmin() {
        string name, phone, email, password, nationaid;
        cin.ignore();
        cout << "Enter admin name: ";
        getline(cin >> ws, name);
        cout << "Enter phone: ";
        getline(cin >> ws, phone);
        cout << "Enter email: ";
        getline(cin >> ws, email);
        cout << "Enter password: ";
        getline(cin >> ws, password);
        cout << "Enter national ID: ";
        getline(cin >> ws, nationaid);
        int newId = FileHelper::getLastId("LastId.txt") + 1;
        FileHelper::saveLastId("LastId.txt", newId);
        try {
            Admin newAdmin(name, newId, password, phone, email, nationaid);
            FileManager::addAdmin(newAdmin);
            cout << "Registration successful your new Admin ID is" << newId << endl;
            return new Admin(newAdmin);
        }
        catch (exception& e) {
            cerr << e.what() << endl;
            return nullptr;
        }
    }
    static bool adminOptions(Admin* admin) {
        int choice;
        do {
            printAdminMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
            case 1: {
                cout << "\033[2J\033[1;1H";
                string name, phone, email, password, nationaid;
                double balance;
                cin.ignore();
                cout << "Enter client name: ";
                getline(cin >> ws, name);
                cout << "Enter phone: ";
                getline(cin >> ws, phone);
                cout << "Enter email: ";
                getline(cin >> ws, email);
                cout << "Enter password: ";
                getline(cin >> ws, password);
                cout << "Enter national ID: ";
                getline(cin >> ws, nationaid);
                cout << "Enter initial balance: ";
                while (!(cin >> balance) || balance < 0) {
                    cout << "Invalid balance! Please enter a non-negative value: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                int newId = FileHelper::getLastId("LastId.txt") + 1;
                FileHelper::saveLastId("LastId.txt", newId);
                Client newClient(name, newId, phone, email, password, nationaid, balance);
                admin->addClient(newClient);
                FileManager::addClient(newClient);
                cout << "Client added successfully! New Client ID: " << newId << endl;
                break;
            }
            case 2: {
                cout << "\033[2J\033[1;1H";
                int clientId;
                cout << "Enter client ID to search: ";
                cin >> clientId;
                Client* client = admin->searchClient(clientId);
                if (client) {
                    client->displayInfo();
                }
                else {
                    cout << "Client not found!" << endl;
                }
                break;
            }
            case 3: {
                cout << "\033[2J\033[1;1H";
                int clientId;
                cout << "enter client id: ";
                cin >> clientId;
                Client* client = nullptr;
                for (auto& c : allClients) {
                    if (c.getId() == clientId) {
                        client = &c;
                        break;
                    }
                }
                if (client) {
                    string newName, newPassword, newPhone, newEmail, newNationaid;
                    double newBalance;
                    cout << "Enter new name: ";
                    getline(cin >> ws, newName);
                    cout << "Enter new password: ";
                    getline(cin >> ws, newPassword);
                    cout << "Enter new phone: ";
                    getline(cin >> ws, newPhone);
                    cout << "Enter new email: ";
                    getline(cin >> ws, newEmail);
                    cout << "Enter new national ID: ";
                    getline(cin >> ws, newNationaid);
                    cout << "Enter new balance: ";
                    cin >> newBalance;
                    admin->editClient(clientId, newName, newPassword, newBalance);
                }
                else {
                    cout << "client not found" << endl;
                }
                break;
            }
            case 4: {
                cout << "\033[2J\033[1;1H";
                if (allClients.empty()) {
                    cout << "No clients available!" << endl;
                }
                else {
                    cout << "Listing all clients:" << endl;
                    for (const auto& client : allClients) {
                        cout << "ID: " << client.getId()
                            << ", Name: " << client.getName()
                            << ", Balance: " << client.getBalance() << endl;
                    }
                }
                break;
            }
            case 5: {
                cout << "\033[2J\033[1;1H";
                admin->listClients();
                break;
            }
            case 6: {
                cout << "\033[2J\033[1;1H";
                int clientId;
                cout << "Enter client ID to remove: ";
                cin >> clientId;
                Client* client = admin->searchClient(clientId);
                if (client) {
                    admin->removeClient(clientId);
                    cout << "Client removed successfully!" << endl;
                }
                else {
                    cout << "Client not found!" << endl;
                }
                break;
            }
            case 7: {
                cout << "\033[2J\033[1;1H";
                admin->reviewLoanRequests();
                break;
            }
            case 8: {
                cout << "\033[2J\033[1;1H";
                string name, phone, email, password, nationaid;
                double salary;
                cin.ignore();
                cout << "Enter employee name: ";
                getline(cin >> ws, name);
                cout << "Enter phone: ";
                getline(cin >> ws, phone);
                cout << "Enter email: ";
                getline(cin >> ws, email);
                cout << "Enter password: ";
                getline(cin >> ws, password);
                cout << "Enter national ID: ";
                getline(cin >> ws, nationaid);
                cout << "Enter salary: ";
                while (!(cin >> salary) || salary <= 0) {
                    cout << "Invalid salary! Enter a positive value: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                int newId = FileHelper::getLastId("LastId.txt") + 1;
                FileHelper::saveLastId("LastId.txt", newId);
                Employee newEmployee(name, newId, password, phone, email, nationaid, salary);
                admin->addEmployee(newEmployee);
                FileManager::addEmployee(newEmployee);
                cout << "Employee added successfully! New Employee ID: " << newId << endl;
                break;
            }
            case 9: {
                cout << "\033[2J\033[1;1H";
                int empId;
                cout << "Enter employee ID to search: ";
                cin >> empId;
                Employee* employee = admin->searchEmployee(empId);
                if (employee) {
                    employee->displayInfo();
                }
                else {
                    cout << "Employee not found!" << endl;
                }
                break;
            }
            case 10: {
                cout << "\033[2J\033[1;1H";
                int empId;
                cout << "Enter employee ID to edit: ";
                cin >> empId;
                Employee* employee = admin->searchEmployee(empId);
                if (employee) {
                    string newName, newPassword, newPhone, newEmail, newNationaid;
                    double newSalary;
                    cout << "Enter new name: ";
                    getline(cin >> ws, newName);
                    cout << "Enter new password: ";
                    getline(cin >> ws, newPassword);
                    cout << "Enter new phone: ";
                    getline(cin >> ws, newPhone);
                    cout << "Enter new email: ";
                    getline(cin >> ws, newEmail);
                    cout << "Enter new national ID: ";
                    getline(cin >> ws, newNationaid);
                    cout << "Enter new salary: ";
                    while (!(cin >> newSalary) || newSalary <= 0) {
                        cout << "Invalid salary! Enter a positive value: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    admin->editEmployee(empId, newName, newPassword, newSalary);
                    cout << "Employee information updated successfully!" << endl;
                }
                else {
                    cout << "Employee not found!" << endl;
                }
                break;
            }
            case 11: {
                cout << "\033[2J\033[1;1H";
                int employeeId;
                cout << "Enter employee id to display info: ";
                cin >> employeeId;
                Employee* employee = nullptr;
                for (auto& employeeid : allEmployees) {
                    if (employeeid.getId() == employeeId) {
                        employee = &employeeid;
                        break;
                    }
                }
                if (employee) {
                    employee->displayInfo();
                }
                else {
                    cout << "Employee not found" << endl;
                }
                break;
            }
            case 12: {
                cout << "\033[2J\033[1;1H";
                admin->listEmployees();
                break;
            }
            case 13: {
                cout << "\033[2J\033[1;1H";
                int empId;
                cout << "Enter employee ID to remove: ";
                cin >> empId;
                Employee* employee = admin->searchEmployee(empId);
                if (employee) {
                    admin->removeEmployee(empId);
                    cout << "Employee removed successfully!" << endl;
                }
                else {
                    cout << "Employee not found!" << endl;
                }
                break;
            }
            case 14: {
                cout << "\033[2J\033[1;1H";
                cout << "Returning to main menu" << endl;
                return true;
            }
            default: {
                cout << "\033[2J\033[1;1H";
                cout << "Invalid choice try again" << endl;
            }
            }
        } while (choice != 14);
        return true;
    }
};
