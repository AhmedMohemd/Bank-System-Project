#include <iostream>
#include "Screens.h"
#include "ClientManager.h"
#include "EmployeeManager.h"
#include "AdminManager.h"
#include "FileManager.h" 
using namespace std;
int main() {
    Screens::printWelcome();
    FileManager::getAllClients();
    FileManager::getAllEmployees();
    FileManager::getAllAdmins();
    while (true) {
        Screens::loginOptions();
        int category = Screens::loginAs();
        int subChoice;
        switch (category) {
        case 1: {
            cout << "\033[2J\033[1;1H";
            cout << "\nClient Options:" << endl;
            cout << "1: Login" << endl;
            cout << "2: Create new account" << endl;
            int attempts = 0;
            while (attempts < 3) {
                cout << "Enter your choice: ";
                cin >> subChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number" << endl;
                }
                else if (subChoice == 1) {
                    Client* client = ClientManager::login();
                    if (client != nullptr) {
                        ClientManager::clientOptions(client);
                        delete client;
                    }
                    break;
                }
                else if (subChoice == 2) {
                    Client* client = ClientManager::registerClient();
                    if (client != nullptr) {
                        ClientManager::clientOptions(client);
                        delete client;
                    }
                    break;
                }
                else {
                    attempts++;
                    cout << "Invalid choice, enter 1 or 2 (" << attempts << "/3 )" << endl;
                    if (attempts >= 3) {
                        cout << "Too many invalid attempts! Returning to main menu." << endl;
                        break;
                    }
                }
            }
            break;
        }
        case 2: {
            cout << "\033[2J\033[1;1H";
            cout << "\nEmployee Options:" << endl;
            cout << "1: Login" << endl;
            cout << "2: Create new account" << endl;
            int attempts = 0;
            while (attempts < 3) {
                cout << "Enter your choice: ";
                cin >> subChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number." << endl;
                }
                else if (subChoice == 1) {
                    Employee* employee = EmployeeManager::login();
                    if (employee != nullptr) {
                        EmployeeManager::employeeOptions(employee);
                        delete employee;
                    }
                    break;
                }
                else if (subChoice == 2) {
                    Employee* employee = EmployeeManager::registerEmployee();
                    if (employee != nullptr) {
                        EmployeeManager::employeeOptions(employee);
                        delete employee;
                    }
                    break;
                }
                else {
                    attempts++;
                    cout << "Invalid choice, enter 1 or 2 (" << attempts << "/3)" << endl;
                    if (attempts >= 3) {
                        cout << "Too many invalid attempts! Returning to main menu." << endl;
                        break;
                    }
                }
            }
            break;
        }
        case 3: {
            cout << "\033[2J\033[1;1H";
            cout << "\nAdmin Options:" << endl;
            cout << "1: Login" << endl;
            cout << "2: Create new account" << endl;
            int attempts = 0;
            while (attempts < 3) {
                cout << "Enter your choice: ";
                cin >> subChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number." << endl;
                }
                else if (subChoice == 1) {
                    Admin* admin = AdminManager::login();
                    if (admin != nullptr) {
                        AdminManager::adminOptions(admin);
                        delete admin;
                    }
                    break;
                }
                else if (subChoice == 2) {
                    Admin* admin = AdminManager::registerAdmin();
                    if (admin != nullptr) {
                        AdminManager::adminOptions(admin);
                        delete admin;
                    }
                    break;
                }
                else {
                    attempts++;
                    cout << "Invalid choice, enter 1 or 2 (" << attempts << "/3)" << endl;
                    if (attempts >= 3) {
                        cout << "Too many invalid attempts! Returning to main menu." << endl;
                        break;
                    }
                }
            }
            break;
        }
        case 4:
            cout << "Exiting Program..." << endl;
            return 0;
        default:
            cout << "Invalid category selected" << endl;
            break;
        }
    }
}
