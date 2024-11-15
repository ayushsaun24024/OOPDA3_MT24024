// main.cpp
#include "railway.h"
#include <sstream>
#include <limits>
using namespace std;

template<typename T>
T getInput(const string& prompt) {
    T value;
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw RailwayException("Invalid input");
    }
    return value;
}

vector<int> getNumberList(const string& prompt) {
    vector<int> numbers;
    string input;
    cout << prompt;
    cin.ignore();
    getline(cin, input);
    
    stringstream ss(input);
    int number;
    while (ss >> number) {
        if (number <= 0) {
            throw RailwayException("Numbers must be positive");
        }
        numbers.push_back(number);
    }
    
    if (numbers.empty()) {
        throw RailwayException("At least one number must be provided");
    }
    return numbers;
}

void displayMenu() {
    cout << "\n=== Railway Management System ===\n"
              << "1. Add a station\n"
              << "2. Add platforms to station\n"
              << "3. Add lines to platform\n"
              << "4. Add train schedule\n"
              << "5. View entire system\n"
              << "6. Exit\n"
              << "Enter your choice: ";
}

int main() {
    RailwaySystem<string> railway;
    
    while (true) {
        try {
            displayMenu();
            int choice = getInput<int>("");

            switch (choice) {
                case 1: {
                    string id = getInput<string>("Enter station ID: ");cin.ignore(); // Clear the newline from previous input
                    string name;
                    cout << "Enter station name: ";
                    getline(cin, name);
                    railway.addStation(id, name);
                    cout << "Station added successfully!\n";
                    break;
                }
                case 2: {
                    string id = getInput<string>("Enter station ID: ");
                    auto station = railway.findStation(id);
                    if (!station) {
                        throw RailwayException("Station not found");
                    }
                    auto platformNumbers = getNumberList("Enter platform numbers (space-separated): ");
                    station->addPlatforms(platformNumbers);
                    cout << "Platforms added successfully!\n";
                    break;
                }
                case 3: {
                    string id = getInput<string>("Enter station ID: ");
                    auto station = railway.findStation(id);
                    if (!station) {
                        throw RailwayException("Station not found");
                    }
                    int platformNumber = getInput<int>("Enter platform number: ");
                    auto* platform = station->findPlatform(platformNumber);
                    if (!platform) {
                        throw RailwayException("Platform not found");
                    }
                    auto lineNumbers = getNumberList("Enter line numbers (space-separated): ");
                    platform->addLines(lineNumbers);
                    cout << "Lines added successfully!\n";
                    break;
                }
                case 4: {
                    string id = getInput<string>("Enter station ID: ");
                    auto station = railway.findStation(id);
                    if (!station) {
                        throw RailwayException("Station not found");
                    }
                    
                    int platformNumber = getInput<int>("Enter platform number: ");
                    int lineNumber = getInput<int>("Enter line number: ");
                    int hours = getInput<int>("Enter hours (0-23): ");
                    int minutes = getInput<int>("Enter minutes (0-59): ");
                    
                    string trainType;
                    cout << "Enter train type (S for Stopping, T for Through): ";
                    cin >> trainType;
                    
                    bool isStoppingTrain = (trainType == "S" || trainType == "s");
                    
                    station->addTrainSchedule(platformNumber, lineNumber, Time(hours, minutes), isStoppingTrain);
                    cout << "Train schedule added successfully!\n";
                    break;
                }
                case 5: {
                    railway.displayAllStations();
                    break;
                }
                case 6: {
                    cout << "Thank you for using Railway Management System!\n";
                    return 0;
                }
                default: {
                    throw RailwayException("Invalid choice");
                }
            }
        }
        catch (const RailwayException& e) {
            cerr << "Error: " << e.what() << "\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (const exception& e) {
            cerr << "Unexpected error: " << e.what() << "\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return 0;
}
