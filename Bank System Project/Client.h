#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>
#include "Person.h"
#include "Validation.h"
using namespace std;
class Client : public Person {
private:
    double balance;
    double loanAmount;
    bool loanApproved;
    vector<string> transactions;
    string getCurrentTime() const {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        ostringstream oss;
        oss << setfill('0')
            << setw(2) << ltm.tm_mday << "/"
            << setw(2) << (ltm.tm_mon + 1) << "/"
            << (1900 + ltm.tm_year) << " "
            << setw(2) << ltm.tm_hour << ":"
            << setw(2) << ltm.tm_min << ":"
            << setw(2) << ltm.tm_sec;
        return oss.str();
    }
    void logTransaction(const string& type, double amount, const string& details) {
        ostringstream oss;
        oss << getCurrentTime() << " - " << type << ": " << amount;
        if (!details.empty())
            oss << " (" << details << ")";
        transactions.push_back(oss.str());
        ofstream file("Transactions.txt", ios::app);
        if (file.is_open()) {
            file << getCurrentTime() << "," << id << "," << name << "," << type << "," << amount << "," << details << endl;
            file.close();
        }
    }
    void updateClientFile() {
        ifstream inFile("Clients.txt");
        ofstream outFile("Temp.txt");
        string line;
        while (getline(inFile, line)) {
            vector<string> tokens;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (stoi(tokens[0]) == this->id) {
                outFile << tokens[0] << "," << tokens[1] << "," << tokens[2] << ","
                    << tokens[3] << "," << tokens[4] << "," << tokens[5] << ","
                    << fixed << setprecision(2) << this->balance << endl;
            }
            else {
                outFile << line << endl;
            }
        }
        inFile.close();
        outFile.close();
        remove("Clients.txt");
        if (rename("Temp.txt", "Clients.txt") != 0) {
            perror("Error renaming file");
        }
    }
public:
    Client() {
        balance = 0;
        loanAmount = 0;
        loanApproved = false;
    }
    Client(const string& name, int id, const string& phone, const string& email, const string& password, const string& nationaid, double balance)
        : Person(name, id, phone, email, password, nationaid) {
        setBalance(balance);
    }
    void setBalance(double amount) {
        if (Validation::checkBalance(amount))
            this->balance = amount;
    }
    void setLoanAmount(double loanAmount) {
        this->loanAmount = loanAmount;
    }
    double getBalance() const {
        return this->balance;
    }
    double getLoanAmount() const {
        return this->loanAmount;
    }
    void deposit(double amount) {
        if (Validation::checkAmount(amount)) {
            balance += amount;
            cout << "Deposit of " << amount << " successful" << endl;
			updateClientFile();
            logTransaction("Deposit", amount, "New balance: " + to_string(balance));
        }
    }
    void withdraw(double amount) {
        if (Validation::checkAmount(amount) && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal of " << amount << " successful" << endl;
            updateClientFile();
            logTransaction("Withdraw", amount, "New balance: " + to_string(balance));
        }
        else
            throw invalid_argument("ERROR: Insufficient balance");
    }
    void transferMoney(Client& account, double amount) {
        if (Validation::checkAmount(amount) && amount <= balance) {
            balance -= amount;
            account.deposit(amount);
            cout << "Successfully transferred " << amount << " to " << account.getName() << endl;
            updateClientFile();
            logTransaction("Transfer", amount, "Transferred to: " + account.getName());
        }
        else
            throw invalid_argument("ERROR: Insufficient balance");
    }
    void applyForLoan(double amount) {
        if (amount <= 0)
            throw invalid_argument("ERROR: Loan amount must be positive");
        ofstream requestFile("LoanRequests.txt", ios::app);
        if (requestFile.is_open()) {
            requestFile << "ClientID: " << getId() << ", Amount: " << amount << ", Status: Pending" << endl;
            requestFile.close();
            cout << "Loan application for amount " << amount << " submitted and recorded." << endl;
        }
        else {
            cout << "Error: Unable to open LoanRequests.txt file." << endl;
        }
    }
    void payLoan(double paymentAmount) {
        ifstream loansFile("Loans.txt");
        ofstream tempFile("Temp.txt");
        string line;
        bool found = false;
        double loanAmount = 0, remainingLoan = 0;
        if (!loansFile.is_open()) {
            cout << "Error: Unable to open Loans.txt file." << endl;
            return;
        }
        while (getline(loansFile, line)) {
            if (line.find("ClientID: " + to_string(getId())) != string::npos &&
                line.find("Status: Approved") != string::npos) {
                size_t amountPos = line.find("Amount: ") + 8;
                size_t commaPos = line.find(",", amountPos);
                loanAmount = stod(line.substr(amountPos, commaPos - amountPos));
                remainingLoan = loanAmount;
                found = true;
                if (paymentAmount > getBalance()) {
                    cout << "ERROR: Insufficient balance to make this payment." << endl;
                    tempFile << line << endl;  
                    continue;
                }
                if (paymentAmount > loanAmount) {
                    cout << "ERROR: You cannot pay more than your remaining loan balance." << endl;
                    tempFile << line << endl;  
                    continue;
                }
                withdraw(paymentAmount);
                remainingLoan -= paymentAmount;
                if (remainingLoan == 0) {
                    tempFile << "ClientID: " << getId() << ", Amount: " << loanAmount << ", Status: Completed" << endl;
                    cout << "Loan fully paid! Status updated to Completed." << endl;
                }
                else {
                    tempFile << "ClientID: " << getId() << ", Amount: " << remainingLoan << ", Status: Approved" << endl;
                    cout << "Payment received. Remaining loan balance: " << remainingLoan << endl;
                }
            }
            else {
                tempFile << line << endl;
            }
        }
        loansFile.close();
        tempFile.close();
        if (found) {
            remove("Loans.txt");
            if (rename("Temp.txt", "Loans.txt") != 0) {
                perror("Error renaming file Loans.txt");
            }
            else {
                cout << "Loan file updated successfully." << endl;
            }
        }
        else {
            cout << "No active loan found." << endl;
        }
    }
    void printTransactionHistory() {
        ifstream file("Transactions.txt");
        if (!file.is_open()) {
            cout << "Error: Unable to open Transactions.txt" << endl;
            return;
        }
        string line;
        bool found = false;
        cout << "Transaction History" << endl;
        cout << "--------------------------" << endl;
        while (getline(file, line)) { 
            cout << line << endl;   
            found = true;
        }
        if (!found) {
            cout << "No transactions found" << endl;
        }
        cout << "--------------------------" << endl;
        file.close();
    }
    void approveLoan() {
        loanApproved = true;
        cout << "Loan Approved for client: " << name << endl;
    }
    void rejectLoan() {
        loanApproved = false;
        cout << "Loan Rejected for client: " << name << endl;
    }
    void updatePersonalInfo(const string& newName, const string& newPhone, const string& newEmail, const string& newPassword, const string& newNationaid) {
        setName(newName);
        setPhone(newPhone);
        setEmail(newEmail);
        setPassword(newPassword);
        setNationaid(newNationaid);
        cout << "Client information updated successfully." << endl;
		updateClientFile();
    }
    void checkBalanceDisplay() const {
        cout << "Balance: " << balance << endl;
    }
    void displayInfo() const {
        cout << "|Client Info:" << endl;
        cout << "-----------------" << endl;
        cout << "|Name: " << name << endl;
        cout << "|ID: " << id << endl;
        cout << "|Password: " << password << endl;
        cout << "|Phone: " << phone << endl;
        cout << "|Email: " << email << endl;
        cout << "|Balance: " << balance << endl;
        cout << "|Loan Approved: " << (loanApproved ? "Yes" : "No") << endl;
        cout << "|National ID: " << nationaid << endl;
        cout << "-----------------" << endl;

    }
};

static vector<Client> allClients;