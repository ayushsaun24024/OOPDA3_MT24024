// railway.hpp
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <sstream>
using namespace std;


// Custom exceptions
class RailwayException : public runtime_error {
public:
    explicit RailwayException(const string& message) : runtime_error(message) {}
};

class TimeConflictException : public RailwayException {
public:
    explicit TimeConflictException(const string& message) : RailwayException(message) {}
};

// Time utility class
class Time {
private:
    int hours;
    int minutes;

public:
    Time(int h = 0, int m = 0) : hours(h), minutes(m) {
        if (h < 0 || h > 23 || m < 0 || m > 59) {
            throw RailwayException("Invalid time format");
        }
    }

    bool operator<(const Time& other) const {
        return (hours * 60 + minutes) < (other.hours * 60 + other.minutes);
    }

    bool operator==(const Time& other) const {
        return hours == other.hours && minutes == other.minutes;
    }

    int getDifference(const Time& other) const {
        return abs((hours * 60 + minutes) - (other.hours * 60 + other.minutes));
    }

    string toString() const {
        stringstream ss;
        ss << setfill('0') << setw(2) << hours << ":" 
           << setfill('0') << setw(2) << minutes;
        return ss.str();
    }
};

// Train schedule class
class TrainSchedule {
public:
    Time time;
    bool isStoppingTrain;

    TrainSchedule(const Time& t, bool stopping) : time(t), isStoppingTrain(stopping) {}

    bool operator==(const TrainSchedule& other) const {
        return time == other.time && isStoppingTrain == other.isStoppingTrain;
    }
};

// Forward declaration
class Platform;

// Line class
class Line {
private:
    int lineNumber;
    vector<TrainSchedule> schedules;

public:
    explicit Line(int num) : lineNumber(num) {}

    bool canAddTrain(const Time& newTime, bool isStoppingTrain) const {
        for (const auto& schedule : schedules) {
            int timeDiff = schedule.time.getDifference(newTime);
            if (isStoppingTrain || schedule.isStoppingTrain) {
                if (timeDiff < 30) return false;
            } else {
                if (timeDiff < 10) return false;
            }
        }
        return true;
    }

    void addTrain(const Time& time, bool isStoppingTrain);

    int getLineNumber() const { return lineNumber; }
    const vector<TrainSchedule>& getSchedules() const { return schedules; }
};

// Platform class
class Platform {
private:
    int platformNumber;
    vector<unique_ptr<Line>> lines;

public:
    explicit Platform(int num) : platformNumber(num) {
        if (num <= 0) {
            throw RailwayException("Platform number must be positive");
        }
    }

    void addLine(int lineNumber);

    void addLines(const vector<int>& lineNumbers);

    Line* findLine(int lineNumber);

    int getPlatformNumber() const { return platformNumber; }
    const vector<unique_ptr<Line>>& getLines() const { return lines; }
};

// Station class template
template<typename T>
class RailwayStation {
private:
    T id;
    string name;
    vector<unique_ptr<Platform>> platforms;

public:
    RailwayStation(T stationId, string stationName) 
        : id(move(stationId)), name(move(stationName)) {}

    void addPlatform(int platformNumber) {
        if (findPlatform(platformNumber)) {
            throw RailwayException("Platform already exists");
        }
        platforms.push_back(make_unique<Platform>(platformNumber));
    }

    void addPlatforms(const vector<int>& platformNumbers) {
        if (platformNumbers.empty()) {
            throw RailwayException("No platform numbers provided");
        }
        for (int platformNum : platformNumbers) {
            addPlatform(platformNum);
        }
    }

    Platform* findPlatform(int platformNumber) {
        auto it = find_if(platforms.begin(), platforms.end(),
            [platformNumber](const auto& platform) { 
                return platform->getPlatformNumber() == platformNumber; 
            });
        return it != platforms.end() ? it->get() : nullptr;
    }

    void addTrainSchedule(int platformNumber, int lineNumber, const Time& time, bool isStoppingTrain) {
        auto* platform = findPlatform(platformNumber);
        if (!platform) {
            throw RailwayException("Platform not found");
        }
        auto* line = platform->findLine(lineNumber);
        if (!line) {
            throw RailwayException("Line not found on this platform");
        }
        line->addTrain(time, isStoppingTrain);
    }

    T getId() const { return id; }
    string getName() const { return name; }
    const vector<unique_ptr<Platform>>& getPlatforms() const { return platforms; }
};

// Railway System class
template<typename T>
class RailwaySystem {
private:
    vector<unique_ptr<RailwayStation<T>>> stations;

public:
    void addStation(T id, const string& name) {
        if (findStation(id)) {
            throw RailwayException("Station ID already exists");
        }
        stations.push_back(make_unique<RailwayStation<T>>(id, name));
    }

    RailwayStation<T>* findStation(T id) {
        auto it = find_if(stations.begin(), stations.end(),
            [&id](const auto& station) { return station->getId() == id; });
        return it != stations.end() ? it->get() : nullptr;
    }

    void displayAllStations() const {
        cout << "\n=== Railway System Status ===\n";
        if (stations.empty()) {
            cout << "No stations in the system.\n";
            return;
        }

        for (const auto& station : stations) {
            cout << "\nStation ID: " << station->getId() 
                     << "\nName: " << station->getName() << "\n";
            
            if (station->getPlatforms().empty()) {
                cout << "No platforms in this station.\n";
                continue;
            }

            for (const auto& platform : station->getPlatforms()) {
                cout << "\nPlatform " << platform->getPlatformNumber() << ":\n";
                
                if (platform->getLines().empty()) {
                    cout << "No lines on this platform.\n";
                    continue;
                }

                for (const auto& line : platform->getLines()) {
                    cout << "\nLine " << line->getLineNumber() << " Schedule:\n";
                    cout << setw(10) << "Time" 
                             << setw(15) << "Train Type" << "\n";
                    cout << string(25, '-') << "\n";

                    const auto& schedules = line->getSchedules();
                    for (const auto& schedule : schedules) {
                        cout << setw(10) << schedule.time.toString()
                                 << setw(15) << (schedule.isStoppingTrain ? "Stopping" : "Through")
                                 << "\n";
                    }
                    cout << string(25, '-') << "\n";
                }
            }
        }
    }
};

#include "railway.cpp"