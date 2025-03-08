#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;
class Screens {
public:
    static void printWelcome() {
        int lines = 20;
        int spaces = 60;
        for (int i = 0; i < lines; i++) {
            cout << endl;
        }
        for (int i = 0; i < spaces; i++) {
            cout << " ";
        }
        cout << " W E L C O M E   T O   B A N K   A H M E D" << endl;
        this_thread::sleep_for(chrono::seconds(5));
#ifdef _WIN32
        cout << "\033[2J\033[1;1H";
#else
        cout << "\033[2J\033[H";
#endif
    }
    static void loginOptions() {
        cout << "Welcome" << endl;
        cout << "1:Login as Client" << endl;
        cout << "2:Login as Employee" << endl;
        cout << "3:Login as Admin" << endl;
        cout << "4:Exit" << endl;
    }
    static int loginAs() {
        int choice;
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "invalid choice please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return choice;
    }
    static void invalid(int c) {
        cout << "invalid choice: " << c << " please choose a valid option" << endl;
    }
    static void logout() {
        cout << "Logging out" << endl;
    }
};
