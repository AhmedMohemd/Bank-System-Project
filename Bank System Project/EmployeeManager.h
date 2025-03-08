#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include "Employee.h"
#include "Client.h"
#include "FileManager.h"
#include "FileHelper.h"
using namespace std;
class EmployeeManager {
public:
    static void printEmployeeMenu() {
        cout << "1:Add New Client" << endl;
        cout << "2:Search for Client" << endl;
        cout << "3:List All Clients" << endl;
        cout << "4:Edit Client Info" << endl;
        cout << "5:Review Client Loan" << endl;
        cout << "6:Display client Info" << endl;
        cout << "7:Remove Cllent" << endl;
        cout << "8:Display My Info" << endl;
        cout << "9:logout (Return to Main Menu)" << endl;
    }
    static Employee* login() {
        cout << "\033[2J\033[1;1H";
        int id;
        string inputPassword;
        int idAttempts = 3;
        while (idAttempts > 0) {
            cout << "Enter your Employee ID: ";
            cin >> id;
            bool idExists = false;
            for (const auto& emp : allEmployees) {
                if (emp.getId() == id) {
                    idExists = true;
                    break;
                }
            }
            if (!idExists) {
                idAttempts--;
                cout << "ERROR: ID not found! Please enter a valid Employee ID.\n";
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
            for (auto& emp : allEmployees) {
                if (emp.getId() == id && emp.getPassword() == inputPassword) {
                    cout << "Login Successful" << endl;
                    return new Employee(emp);
                }
            }
            attempts--;
            if (attempts > 0)
                cout << "Invalid credentials attempts left: " << attempts << endl;
        }
        cout << "Too many failed attempts please wait 1 minute" << endl;
        this_thread::sleep_for(chrono::minutes(1));
        return nullptr;
    }
    static Employee* registerEmployee() {
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
        cout << "Enter salary (must be at least 5000): ";
        while (!(cin >> salary) || salary < 5000) {
            cout << "invalid choice Please enter a salary of at least 5000: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        int newId = FileHelper::getLastId("LastId.txt") + 1;
        FileHelper::saveLastId("LastId.txt", newId);
        try {
            Employee newEmployee(name, newId, phone, email, password,nationaid, salary);
            allEmployees.push_back(newEmployee);
            FileManager::addEmployee(newEmployee);
            cout << "Registration successful your new Employee ID is " << newId << endl;
            return new Employee(newEmployee);
        }
        catch (exception& e) {
            cerr << e.what() << endl;
            return nullptr;
        }
    }

    static bool employeeOptions(Employee* employee) {
        int choice;
        do {
            printEmployeeMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
            case 1: {
                cout << "\033[2J\033[1;1H";
                string name;
                int clientId;
                string password, phone, email, nationaid;
                double balance;
                cout << "Enter client name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter client ID: ";
                cin >> clientId;
                cout << "Enter client password: ";
                cin >> password;
                cout << "Enter client phone: ";
                cin >> phone;
                cout << "Enter client email: ";
                cin >> email;
                cout << "Enter client national ID: ";
                cin >> nationaid;
                cout << "Enter client balance: ";
                cin >> balance;
                Client newClient(name, clientId, phone, email, password, nationaid, balance);
                allClients.push_back(newClient);
                employee->addClient(newClient);
                FileManager::addClient(newClient);
                break;
            }
            case 2: {
                cout << "\033[2J\033[1;1H";
                int clientId;
                cout << "Enter client id: ";
                cin >> clientId;
                Client* client = nullptr;
                for (auto& c : allClients) {
                    if (c.getId() == clientId) {
                        client = &c;
                        break;
                    }
                }
                if (client) {
                    client->displayInfo();
                }
                else {
                    cout << "Client not found" << endl;
                }
                break;
            }
            case 3: {
                cout << "\033[2J\033[1;1H";
                employee->listClients();
                break;
            }
            case 4: {
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
                    employee->editClient(clientId, newName, newPassword, newBalance);
                }
                else {
                    cout << "client not found" << endl;
                }
                break;
            }
            case 5: {
                cout << "\033[2J\033[1;1H";
                employee->reviewLoanRequests();
                break;
            }
            case 6: {
                cout << "\033[2J\033[1;1H";
                int clientid;
                cout << "Enter client id to display info: ";
                cin >> clientid;
                Client* client = nullptr;
                for (auto& cli : allClients) {
                    if (cli.getId() == clientid) {
                        client = &cli;
                        break;
                    }
                }
                if (client) {
                    client->displayInfo();
                }
                else
                    cout << "Client not found" << endl;
                break;
            }
            case 7: {
                cout << "\033[2J\033[1;1H";
                int clientid;
                cout << "Enter client id to remove: ";
                cin >> clientid;
                employee->removeClient(clientid);
                break;
            }
            case 8: {
                cout << "\033[2J\033[1;1H";
                employee->displayInfo();
                break;
            }
            case 9: {
                cout << "\033[2J\033[1;1H";
                cout << "Returning to main menu" << endl;
                return true;
            }
            default: {
                cout << "\033[2J\033[1;1H";
                cout << "your choice not found Try again." << endl;
            }
            }
        } while (choice != 9);
        return true;
    }
};
