#include <iostream>
#include <fstream>
#include "calculator.hpp"
using namespace std;

int main() {
    ifstream infile("input.txt");
    string line;

    if (!infile.is_open()) {
        cerr << "Could not open input.txt" << endl;
        return 1;
    }

    while (getline(infile, line)) {
        cout << "Checking: " << line << endl;
        if (isValidDouble(line)) {
            cout << "Valid double." << endl;
            string result = addDoubles(line, "-123.456");
            cout << "Result after adding -123.456: " << result << endl;
        } else {
            cout << "Invalid double number." << endl;
        }
        cout << endl;
    }

    infile.close();
    return 0;
}
