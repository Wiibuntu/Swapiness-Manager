#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

// Function to get the current swappiness value
int getSwappiness() {
    ifstream swappinessFile("/proc/sys/vm/swappiness");
    if (!swappinessFile.is_open()) {
        cerr << "Failed to open swappiness file." << endl;
        return -1;
    }
    int swappiness;
    swappinessFile >> swappiness;
    swappinessFile.close();
    return swappiness;
}

// Function to set the swappiness value
bool setSwappiness(int value) {
    ofstream swappinessFile("/proc/sys/vm/swappiness");
    if (!swappinessFile.is_open()) {
        cerr << "Failed to open swappiness file. Try running as root." << endl;
        return false;
    }
    swappinessFile << value;
    swappinessFile.close();
    return true;
}

// Function to get memory usage
void getMemoryUsage() {
    ifstream meminfoFile("/proc/meminfo");
    if (!meminfoFile.is_open()) {
        cerr << "Failed to open meminfo file." << endl;
        return;
    }

    string line;
    int totalRam = 0, freeRam = 0, totalSwap = 0, freeSwap = 0;

    while (getline(meminfoFile, line)) {
        stringstream ss(line);
        string key;
        int value;
        string unit;

        ss >> key >> value >> unit;
        if (key == "MemTotal:") totalRam = value;
        else if (key == "MemFree:") freeRam = value;
        else if (key == "SwapTotal:") totalSwap = value;
        else if (key == "SwapFree:") freeSwap = value;
    }
    meminfoFile.close();

    cout << "RAM Usage: " << (totalRam - freeRam) / 1024 << " MB / " << totalRam / 1024 << " MB" << endl;
    cout << "Swap Usage: " << (totalSwap - freeSwap) / 1024 << " MB / " << totalSwap / 1024 << " MB" << endl;
}

int main() {
    while (true) {
        cout << "Current Swappiness: " << getSwappiness() << endl;
        getMemoryUsage();

        cout << "\nOptions:" << endl;
        cout << "1. Change swappiness" << endl;
        cout << "2. Refresh" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter new swappiness value (0-100): ";
            int newSwappiness;
            cin >> newSwappiness;

            if (newSwappiness < 0 || newSwappiness > 100) {
                cerr << "Invalid swappiness value. It must be between 0 and 100." << endl;
            } else {
                if (setSwappiness(newSwappiness)) {
                    cout << "Swappiness updated successfully." << endl;
                }
            }
        } else if (choice == 2) {
            continue; // Refresh by re-displaying the menu
        } else if (choice == 3) {
            break; // Exit the program
        } else {
            cerr << "Invalid choice. Please try again." << endl;
        }

        cout << "\n"; // Add some spacing
    }

    return 0;
}
