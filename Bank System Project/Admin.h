#pragma once
#include <iostream>
#include <string>
#include "Person.h"
#include "Client.h"
#include "Employee.h"
using namespace std;
class Admin : public Person {
public:
    Admin() {
    }
    Admin(const string& name, int id, const string& password, const string& phone, const string& email, const string& nationaid)
        : Person(name, id, phone, email, password, nationaid) {}
    void addClient(const Client& client) {
        allClients.push_back(client);
        cout << "Client added successfully by admin." << endl;
    }
    Client* searchClient(int clientId) {
        for (auto& client : allClients) {
            if (client.getId() == clientId)
                return &client;
        }
        cout << "Client not found!" << endl;
        return nullptr;
    }
    void listClients() const {
        if (allClients.empty()) {
            cout << "No clients available!" << endl;
            return;
        }
        cout << "Listing all clients:" << endl;
        cout << "-----------------" << endl;
        for (const auto& client : allClients) {
            cout << "|Name: " << client.getName() << endl;
            cout << "|ID: " << client.getId() << endl;
            cout << "|Balance: " << client.getBalance() << endl;
            cout << "|Gmail: " << client.getEmail() << endl;
            cout << "|NationaNID: " << client.getNationaid() << endl;
            cout << "|Phone: " << client.getPhone() << endl;
            cout << "-----------------" << endl;
        }
    }
    void editClient(int clientId, const string& newName, const string& newPassword, double newBalance) {
        Client* clientToEdit = nullptr;
        for (auto& client : allClients) {
            if (client.getId() == clientId) {
                clientToEdit = &client;
                break;
            }
        }
        if (clientToEdit != nullptr) {
            clientToEdit->setName(newName);
            clientToEdit->setPassword(newPassword);
            clientToEdit->setBalance(newBalance);
            cout << "Client info updated successfully." << endl;
            ofstream clientsFile("Clients.txt");
            if (clientsFile.is_open()) {
                for (const auto& client : allClients) {
                    clientsFile << client.getId() << ","
                        << client.getName() << ","
                        << client.getPhone() << ","
                        << client.getEmail() << ","
                        << client.getPassword() << ","
                        << client.getNationaid() << ","
                        << client.getBalance() << endl;
                }
                clientsFile.close();
                cout << "Clients.txt updated successfully." << endl;
            }
            else {
                cout << "Error: Unable to open Clients.txt file for updating." << endl;
            }
        }
        else {
            cout << "Client not found!" << endl;
        }
    }
    void removeClient(int clientId) {
        Client* clientToRemove = nullptr;
        for (auto& client : allClients) {
            if (client.getId() == clientId) {
                clientToRemove = &client;
                break;
            }
        }
        if (clientToRemove) {
            ofstream outFile("DeletedClients.txt", ios::app);
            if (outFile.is_open()) {
                outFile << clientToRemove->getId() << ","
                    << clientToRemove->getName() << ","
                    << clientToRemove->getPhone() << ","
                    << clientToRemove->getEmail() << ","
                    << clientToRemove->getPassword() << ","
                    << clientToRemove->getNationaid() << ","
                    << clientToRemove->getBalance() << endl;
                outFile.close();
                cout << "Client data moved to DeletedClients.txt successfully." << endl;
            }
            else {
                cout << "Error: Unable to open DeletedClients.txt file." << endl;
                return;
            }
            auto it = remove_if(allClients.begin(), allClients.end(), [&](const Client& c) {
                return c.getId() == clientId;
                });
            allClients.erase(it, allClients.end());
            ofstream clientsFile("Clients.txt");
            if (clientsFile.is_open()) {
                for (const auto& client : allClients) {
                    clientsFile << client.getId() << ","
                        << client.getName() << ","
                        << client.getPhone() << ","
                        << client.getEmail() << ","
                        << client.getPassword() << ","
                        << client.getNationaid() << ","
                        << client.getBalance() << endl;
                }
                clientsFile.close();
                cout << "Clients.txt updated successfully." << endl;
            }
            else {
                cout << "Error: Unable to open Clients.txt file for updating." << endl;
            }
            cout << "Client removed successfully from active clients." << endl;
        }
        else {
            cout << "Client not found!" << endl;
        }
    }
    void approveLoanRequest(int clientId, double loanAmount) {
        bool found = false;
        ifstream requestFile("LoanRequests.txt");
        ofstream tempFile("Temp.txt");
        ofstream loansFile("Loans.txt", ios::app);
        string line;
        while (getline(requestFile, line)) {
            if (line.find("ClientID: " + to_string(clientId)) != string::npos &&
                line.find("Amount: " + to_string(loanAmount)) != string::npos) {
                loansFile << "ClientID: " << clientId << ", Amount: " << loanAmount << ", Status: Approved" << endl;
                cout << "Loan approved and added to Loans.txt." << endl;
                found = true;
            }
            else {
                tempFile << line << endl;
            }
        }
        requestFile.close();
        tempFile.close();
        loansFile.close();
        if (found) {
            remove("LoanRequests.txt");
            if (rename("Temp.txt", "LoanRequests.txt") != 0) {
                perror("Error renaming file LoanRequests.txt");
            }
            else {
                cout << "Loan request file updated successfully." << endl;
            }
            for (auto& client : allClients) {
                if (client.getId() == clientId) {
                    client.deposit(loanAmount);
                    cout << "Loan amount added to client's balance successfully." << endl;
                    break;
                }
            }
        }
        else {
            cout << "Loan request not found." << endl;
        }
    }
    void cleanLoanRequests(int clientId, double loanAmount) {
        ifstream requestFile("LoanRequests.txt");
        ofstream tempFile("Temp.txt");
        string line;
        while (getline(requestFile, line)) {
            if (line.find("ClientID: " + to_string(clientId)) == string::npos || line.find("Amount: " + to_string(loanAmount)) == string::npos) {
                tempFile << line << endl;
            }
        }
        requestFile.close();
        tempFile.close();
        remove("LoanRequests.txt");
        if (rename("Temp.txt", "LoanRequests.txt") != 0) {
            perror("remaning LoanRequests.txt noy found");
        }
    }
    void rejectLoanRequest(int clientId, double loanAmount) {
        ifstream requestFile("LoanRequests.txt");
        string line;
        vector<string> requests;
        while (getline(requestFile, line)) {
            requests.push_back(line);
        }
        requestFile.close();
        bool found = false;
        ofstream loansFile("Loans.txt", ios::app);
        for (auto& req : requests) {
            if (req.find("ClientID: " + to_string(clientId)) != string::npos && req.find("Amount: " + to_string(loanAmount)) != string::npos) {
                loansFile << req.substr(0, req.find("Status")) << ", Status: Rejected" << endl;
                found = true;
                break;
            }
        }
        loansFile.close();
        if (found) {
            cout << "Loan request for client " << clientId << " rejected." << endl;
            cleanLoanRequests(clientId, loanAmount);
        }
        else {
            cout << "Loan request not found." << endl;
        }
    }
    void addEmployee(const Employee& employee) {
        allEmployees.push_back(employee);
        cout << "Employee added successfully by admin." << endl;
    }
    Employee* searchEmployee(int employeeId) {
        for (auto& employee : allEmployees) {
            if (employee.getId() == employeeId)
                return &employee;
        }
        cout << "Employee not found!" << endl;
        return nullptr;
    }
    void editEmployee(int employeeId, const string& newName, const string& newPassword, double newSalary) {
        for (auto& employee : allEmployees) {
            if (employee.getId() == employeeId) {
                employee.setName(newName);
                employee.setPassword(newPassword);
                employee.setSalary(newSalary);
                cout << "Employee info updated successfully." << endl;
                ofstream employeesFile("Employee.txt");
                if (employeesFile.is_open()) {
                    for (const auto& emp : allEmployees) {
                        employeesFile << emp.getId() << ","
                            << emp.getName() << ","
                            << emp.getPhone() << ","
                            << emp.getEmail() << ","
                            << emp.getPassword() << ","
                            << emp.getNationaid() << ","
                            << emp.getSalary() << endl;
                    }
                    employeesFile.close();
                }
                else {
                    cout << "Error: Unable to open Employee.txt file for updating." << endl;
                }
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }
    void listEmployees() const {
        if (allEmployees.empty()) {
            cout << "No employees available!" << endl;
            return;
        }
        cout << "Listing all employees:" << endl;
        cout << "-----------------" << endl;
        for (const auto& employee : allEmployees) {
            cout << "|Name: " << employee.getName() << endl;
            cout << "|ID: " << employee.getId() << endl;
            cout << "|Salary: " << employee.getSalary() << endl;
            cout << "|Gmail: " << employee.getEmail() << endl;
            cout << "|NationaNID: " << employee.getNationaid() << endl;
            cout << "|Phone: " << employee.getPhone() << endl;
            cout << "-----------------" << endl;
        }
    }
    void removeEmployee(int employeeId) {
        for (auto it = allEmployees.begin(); it != allEmployees.end(); ++it) {
            if (it->getId() == employeeId) {
                ofstream outFile("DeletedEmployees.txt", ios::app);
                if (outFile.is_open()) {
                    outFile << it->getId() << ","
                        << it->getName() << ","
                        << it->getPhone() << ","
                        << it->getEmail() << ","
                        << it->getPassword() << ","
                        << it->getNationaid() << ","
                        << it->getSalary() << endl;
                    outFile.close();
                }
                else {
                    cout << "Error: Unable to open DeletedEmployees.txt file." << endl;
                    return;
                }
                allEmployees.erase(it);
                ofstream employeesFile("Employee.txt");
                if (employeesFile.is_open()) {
                    for (const auto& emp : allEmployees) {
                        employeesFile << emp.getId() << ","
                            << emp.getName() << ","
                            << emp.getPhone() << ","
                            << emp.getEmail() << ","
                            << emp.getPassword() << ","
                            << emp.getNationaid() << ","
                            << emp.getSalary() << endl;
                    }
                    employeesFile.close();
                }
                else {
                    cout << "Error: Unable to open Employee.txt file for updating." << endl;
                }

                cout << "Employee removed successfully." << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }
    void displayLoanRequests() {
        ifstream requestFile("LoanRequests.txt");
        string line;
        if (requestFile.is_open()) {
            cout << "Loan Requests: " << endl;
            while (getline(requestFile, line)) {
                cout << line << endl;
            }
            requestFile.close();
        }
        else {
            cout << "Error: Unable to open LoanRequests.txt file." << endl;
        }
    }
    void reviewClientLoan(Client& client) {
        cout << "Admin reviewing loan application for client: " << client.getName() << endl;
        if (client.getLoanAmount() > 0) {
            if (client.getBalance() >= 1500)
                client.approveLoan();
            else
                client.rejectLoan();
        }
        else {
            cout << "No loan application found for client: " << client.getName() << endl;
        }
    }
    void reviewLoanRequests() {
        ifstream requestFile("LoanRequests.txt");
        vector<string> requests;
        string line;
        int index = 1;
        if (!requestFile.is_open()) {
            cout << "Error: Unable to open LoanRequests.txt file." << endl;
            return;
        }
        cout << "Loan Requests:" << endl;
        while (getline(requestFile, line)) {
            requests.push_back(line);
            cout << index << ": " << line << endl;
            index++;
        }
        requestFile.close();
        if (requests.empty()) {
            cout << "No loan requests available." << endl;
            return;
        }
        int choice;
        cout << "Enter the number of the loan request to review (or 0 to cancel): ";
        cin >> choice;
        if (choice == 0 || choice > requests.size()) {
            cout << "Invalid choice or canceled." << endl;
            return;
        }
        string selectedRequest = requests[choice - 1];
        int clientId;
        double loanAmount;
        size_t idPos = selectedRequest.find("ClientID: ") + 9;
        size_t amountPos = selectedRequest.find(", Amount: ") + 9;
        clientId = stoi(selectedRequest.substr(idPos, selectedRequest.find(",", idPos) - idPos));
        loanAmount = stod(selectedRequest.substr(amountPos, selectedRequest.find(",", amountPos) - amountPos));
        int action;
        cout << "1: Approve Loan" << endl;
        cout << "2: Reject Loan" << endl;
        cout << "Enter your choice: ";
        cin >> action;
        ofstream loansFile("Loans.txt", ios::app);
        if (!loansFile.is_open()) {
            cout << "Error: Unable to open Loans.txt file." << endl;
            return;
        }
        if (action == 1) {
            loansFile << selectedRequest.substr(0, selectedRequest.find("Status")) << ", Status: Approved" << endl;
            cout << "Loan request approved successfully!" << endl;
        }
        else if (action == 2) {
            loansFile << selectedRequest.substr(0, selectedRequest.find("Status")) << ", Status: Rejected" << endl;
            cout << "Loan request rejected successfully!" << endl;
        }
        else {
            cout << "Invalid choice." << endl;
            return;
        }
        loansFile.close();
        ofstream tempFile("Temp.txt");
        for (int i = 0; i < requests.size(); i++) {
            if (i != choice - 1) {
                tempFile << requests[i] << endl;
            }
        }
        tempFile.close();
        remove("LoanRequests.txt");
        if (rename("Temp.txt", "LoanRequests.txt") != 0) {
            perror("remaning LoanRequests.txt noy found");
        }
    }
    void displayInfo() const {
        cout << "Admin Info:" << endl;
        cout << "-----------------" << endl;
        cout << "|Name: " << name << endl;
        cout << "|ID: " << id << endl;
        cout << "|Phone: " << phone << endl;
        cout << "|Email: " << email << endl;
        cout << "|National ID: " << nationaid << endl;
        cout << "-----------------" << endl;
    }
};
static vector<Admin> allAdmins;
