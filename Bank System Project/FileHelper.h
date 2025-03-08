#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "Parser.h"
using namespace std;
class FileHelper {
public:
    static vector<vector<string>> readFile(const string& filename) {
        vector<vector<string>> data;
        ifstream file(filename);
        if (!file) {
            cerr << "Error file " << filename << " not found Creating a new one" << endl;
            ofstream newFile(filename);
            newFile.close();
            return data;
        }
        string line;
        while (getline(file, line)) {
            vector<string> row = Parser::split(line);
            data.push_back(row);
        }
        file.close();
        return data;
    }
    static void writeFile(const string& filename, const vector<vector<string>>& data) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error ould not open file " << filename << " for writing" << endl;
            return;
        }
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); i++) {
                file << row[i];
                if (i != row.size() - 1)
                    file << ",";
            }
            file << endl;
        }
        file.close();
    }
    static void appendToFile(const string& filename, const vector<string>& row) {
        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error Could not open file " << filename << " for appending" << endl;
            return;
        }
        for (size_t i = 0; i < row.size(); i++) {
            file << row[i];
            if (i != row.size() - 1)
                file << ",";
        }
        file << endl;
        file.close();
    }
    static int getLastId(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "File " << filename << " not found! Creating a new one" << endl;
            ofstream newFile(filename);
            newFile << "0" << endl;
            newFile.close();
            return 0;
        }
        string line;
        int lastId = 0;
        if (getline(file, line)) {
            try {
                lastId = stoi(line);
            }
            catch (exception& e) {
                cerr << "Error invalid ID format in " << e.what() << filename << " Setting last ID to 0" << endl;
                lastId = 0;
            }
        }
        file.close();
        return lastId;
    }
    static void saveLastId(const string& filename, int id) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error Could not open file " << filename << " for writing" << endl;
            return;
        }
        file << id << endl;
        file.close();
    }
};
