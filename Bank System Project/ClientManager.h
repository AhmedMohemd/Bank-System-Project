#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include "Client.h"
#include "FileManager.h"
#include "FileHelper.h"
using namespace std;
class ClientManager {
public:
    static void printClientMenu() {
        cout << "1:Deposit" << endl;
        cout << "2:Withdraw" << endl;
        cout << "3:Transfer Money" << endl;
        cout << "4:Check Balance" << endl;
        cout << "5:Apply for Loan" << endl;
        cout << "6:Print Transaction History" << endl;
        cout << "7:Update Personal Info" << endl;
        cout << "8:Pay Loan" << endl;
        cout << "9:Display my info" << endl;
        cout << "10:Logout (Return to Main Menu)" << endl;
    }
    /*static void updatePassword(Person* person) {
        string newPassword;
        cout << "Enter new password: ";
        cin >> newPassword;
        person->setPassword(newPassword);
        cout << "Password updated successfully" << endl;
    }*/
    static Client* login() {
        cout << "\033[2J\033[1;1H";
        int id;
        string inputPassword;
        int idAttempts = 3;
        while (idAttempts > 0) {
            cout << "Enter your Client ID: ";
            cin >> id;
            bool idExists = false;
            for (const auto& client : allClients) {
                if (client.getId() == id) {
                    idExists = true;
                    break;
                }
            }
            if (!idExists) {
                idAttempts--;
                cout << "ERROR: ID not found! Please enter a valid Client ID.\n";
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
            for (auto& client : allClients) {
                if (client.getId() == id && client.getPassword() == inputPassword) {
                    cout << "Login Successful" << endl;
                    return new Client(client);
                }
            }
            attempts--;
            if (attempts > 0)
                cout << "Invalid credentials. Attempts left: " << attempts << endl;
        }
        cout << "Too many failed attempts please wait 1 minute" << endl;
        this_thread::sleep_for(chrono::minutes(1));
        return nullptr;
    }
    static Client* registerClient() {
        string name, phone, email, password, nationaid;
        double balance;
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin >> ws, name);
        cout << "Enter phone: ";
        getline(cin >> ws, phone);
        cout << "Enter email: ";
        getline(cin >> ws, email);
        cout << "Enter password: ";
        getline(cin >> ws, password);
        cout << "Enter national ID: ";
        getline(cin >> ws, nationaid);
        cout << "Enter initial balance (must be at least 50): ";
        while (!(cin >> balance) || balance < 50) {
            cout << "invalid choice Please enter a balance of at least 1500: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        int newId = FileHelper::getLastId("LastId.txt") + 1;
        FileHelper::saveLastId("LastId.txt", newId);
        try {
            Client newClient(name, newId, phone, email, password, nationaid, balance);
            allClients.push_back(newClient);
            FileManager::addClient(newClient);
            cout << "Registration successful your new ID is " << newId << endl;
            return new Client(newClient);
        }
        catch (exception& e) {
            cerr << e.what() << endl;
            return nullptr;
        }
    }
    static bool clientOptions(Client* client) {
        int choice;
        do {
            printClientMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
            case 1: {
                cout << "\033[2J\033[1;1H";
                double amount;
                cout << "Enter amount to deposit: ";
                while (!(cin >> amount) || amount <= 0) {
                    cout << "invalid choice Please enter a positive number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                client->deposit(amount);
                break;
            }
            case 2: {
                cout << "\033[2J\033[1;1H";
                double amount;
                cout << "Enter amount to withdraw: ";
                while (!(cin >> amount) || amount <= 0) {
                    cout << "invalid choice Please enter a positive number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                try {
                    client->withdraw(amount);
                }
                catch (exception& e) {
                    cerr << e.what() << endl;
                }
                break;
            }
            case 3: {
                cout << "\033[2J\033[1;1H";
                int recipientId;
                double amount;
                cout << "Enter recipient ID: ";
                cin >> recipientId;
                cout << "Enter amount to transfer: ";
                while (!(cin >> amount) || amount <= 0) {
                    cout << "invalid choice Please enter a positive number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                bool transferred = false;
                for (auto& recipient : allClients) {
                    if (recipient.getId() == recipientId) {
                        try {
                            client->transferMoney(recipient, amount);
                            transferred = true;
                        }
                        catch (exception& e) {
                            cerr << e.what() << endl;
                        }
                        break;
                    }
                }
                if (!transferred) {
                    cout << "recipient not found or invalid transfer amount" << endl;
                }
                break;
            }
            case 4: {
                cout << "\033[2J\033[1;1H";
                client->checkBalanceDisplay();
                break;
            }
            case 5: {
                cout << "\033[2J\033[1;1H";
                double loanAmount;
                cout << "Enter loan amount: ";
                cin >> loanAmount;
                client->applyForLoan(loanAmount);
                break;
            }
            case 6: {
                cout << "\033[2J\033[1;1H";
                client->printTransactionHistory();
                break;
            }
            case 7: {
                cout << "\033[2J\033[1;1H";
                string newName, newPhone, newEmail, newPassword, newNationaid;
                cout << "Enter new name: ";
                getline(cin >> ws, newName);
                cout << "Enter new phone: ";
                getline(cin >> ws, newPhone);
                cout << "Enter new email: ";
                getline(cin >> ws, newEmail);
                cout << "Enter new password: ";
                getline(cin >> ws, newPassword);
                cout << "Enter new national ID: ";
                getline(cin >> ws, newNationaid);
                client->updatePersonalInfo(newName, newPhone, newEmail, newPassword, newNationaid);
                break;
            }
            case 8: {
                cout << "\033[2J\033[1;1H";
                ifstream loansFile("Loans.txt");
                string line;
                bool hasLoan = false;
                double loanAmount = 0, remainingLoan = 0;
                if (!loansFile.is_open()) {
                    cout << "Error: Unable to open Loans.txt file." << endl;
                    break;
                }
                while (getline(loansFile, line)) {
                    if (line.find("ClientID: " + to_string(client->getId())) != string::npos &&
                        line.find("Status: Approved") != string::npos) {
                        size_t amountPos = line.find("Amount: ") + 8;
                        size_t commaPos = line.find(",", amountPos);
                        loanAmount = stod(line.substr(amountPos, commaPos - amountPos));
                        remainingLoan = loanAmount;
                        hasLoan = true;
                        break;
                    }
                }
                loansFile.close();
                if (!hasLoan) {
                    cout << "You have no active loans to pay." << endl;
                    break;
                }
                cout << "\033[2J\033[1;1H";
                cout << "Your Loan Details:\n";
                cout << "Total Loan Amount: " << loanAmount << endl;
                cout << "Remaining Balance: " << remainingLoan << endl;
                cout << "\n1: Pay Loan" << endl;
                cout << "2: Return to menu" << endl;
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;
                if (choice == 2) {
                    cout << "Returning to main menu..." << endl;
                    break;
                }
                else if (choice != 1) {
                    cout << "Invalid choice! Returning to main menu..." << endl;
                    break;
                }
                double paymentAmount;
                cout << "Enter amount to pay for your loan: ";
                while (!(cin >> paymentAmount) || paymentAmount <= 0) {
                    cout << "Invalid amount! Please enter a positive number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                client->payLoan(paymentAmount);
                break;
            }
            case 9: {
                cout << "\033[2J\033[1;1H";
                client->displayInfo();
                break;
            }
            case 10: {
                cout << "\033[2J\033[1;1H";
                cout << "return to main menu" << endl;
                return true;
            }
            default: {
                cout << "\033[2J\033[1;1H";
                cout << "your choice not found Try again" << endl;
            }
            }
        } while (choice != 10);
        return true;
    }
};
