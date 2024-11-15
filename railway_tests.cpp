// railway_tests.cpp
#include "railway.h"
#include <cassert>
#include <iostream>

class TestRailwaySystem {
private:
    void testTimeClass() {
        std::cout << "Testing Time class...\n";
        
        // Test valid time construction
        Time t1(12, 30);
        assert(t1.toString() == "12:30");
        
        // Test invalid time
        try {
            Time t2(25, 0);
            assert(false && "Should throw exception for invalid hours");
        } catch (const RailwayException&) {}
        
        try {
            Time t3(12, 60);
            assert(false && "Should throw exception for invalid minutes");
        } catch (const RailwayException&) {}
        
        // Test comparison
        Time t4(13, 0);
        assert(t1 < t4);
        
        // Test time difference
        assert(t1.getDifference(t4) == 30);
        
        // Test equality
        Time t5(12, 30);
        assert(t1 == t5);
    }

    void testPlatformAndLine() {
        std::cout << "Testing Platform and Line classes...\n";
        
        // Test platform creation and line addition
        Platform platform(1);
        
        // Test adding multiple lines
        std::vector<int> lineNumbers = {1, 2, 3};
        platform.addLines(lineNumbers);
        
        // Verify all lines were added
        for (int lineNum : lineNumbers) {
            assert(platform.findLine(lineNum) != nullptr);
        }
        
        // Test duplicate line
        try {
            platform.addLine(1);
            assert(false && "Should throw exception for duplicate line");
        } catch (const RailwayException&) {}
        
        // Test train scheduling
        auto* line = platform.findLine(1);
        assert(line != nullptr);
        
        // Test adding trains with different time gaps
        line->addTrain(Time(10, 0), true);  // Stopping train
        line->addTrain(Time(11, 0), true);  // OK - 60 min gap
        line->addTrain(Time(12, 0), false); // OK - Through train
        
        // Test time conflicts
        try {
            line->addTrain(Time(10, 25), true);
            assert(false && "Should throw exception for time conflict with stopping train");
        } catch (const TimeConflictException&) {}
        
        try {
            line->addTrain(Time(12, 5), false);
            assert(false && "Should throw exception for time conflict with through train");
        } catch (const TimeConflictException&) {}
        
        // Test non-existent line
        assert(platform.findLine(99) == nullptr);
        
        // Test platform number getter
        assert(platform.getPlatformNumber() == 1);
        
        // Test schedule retrieval
        const auto& schedules = line->getSchedules();
        assert(schedules.size() == 3);
    }

    void testRailwayStation() {
        std::cout << "Testing RailwayStation class...\n";
        
        RailwayStation<std::string> station("S1", "Central");
        
        // Test adding multiple platforms
        std::vector<int> platformNumbers = {1, 2, 3};
        station.addPlatforms(platformNumbers);
        
        // Verify all platforms were added
        for (int platformNum : platformNumbers) {
            assert(station.findPlatform(platformNum) != nullptr);
        }
        
        // Test duplicate platform
        try {
            station.addPlatform(1);
            assert(false && "Should throw exception for duplicate platform");
        } catch (const RailwayException&) {}
        
        // Add lines to multiple platforms
        auto* platform1 = station.findPlatform(1);
        auto* platform2 = station.findPlatform(2);
        assert(platform1 != nullptr && platform2 != nullptr);
        
        std::vector<int> lineNumbers1 = {1, 2};
        std::vector<int> lineNumbers2 = {3, 4};
        platform1->addLines(lineNumbers1);
        platform2->addLines(lineNumbers2);
        
        // Test train scheduling on different platforms
        station.addTrainSchedule(1, 1, Time(10, 0), true);
        station.addTrainSchedule(1, 2, Time(10, 0), true);  // Same time OK on different lines
        station.addTrainSchedule(2, 3, Time(10, 0), true);  // Same time OK on different platforms
        
        // Test invalid platform/line combinations
        try {
            station.addTrainSchedule(4, 1, Time(10, 0), true);
            assert(false && "Should throw exception for non-existent platform");
        } catch (const RailwayException&) {}
        
        try {
            station.addTrainSchedule(1, 3, Time(10, 0), true);
            assert(false && "Should throw exception for non-existent line on platform");
        } catch (const RailwayException&) {}
        
        // Test station getters
        assert(station.getId() == "S1");
        assert(station.getName() == "Central");
        assert(station.getPlatforms().size() == 3);
    }

    void testRailwaySystem() {
        std::cout << "Testing RailwaySystem class...\n";
        
        RailwaySystem<std::string> railway;
        
        // Test adding stations
        railway.addStation("S1", "Central");
        railway.addStation("S2", "North");
        
        // Test duplicate station
        try {
            railway.addStation("S1", "Another Central");
            assert(false && "Should throw exception for duplicate station ID");
        } catch (const RailwayException&) {}
        
        // Test complete workflow for multiple stations
        auto* station1 = railway.findStation("S1");
        auto* station2 = railway.findStation("S2");
        assert(station1 != nullptr && station2 != nullptr);
        
        // Add platforms to both stations
        std::vector<int> platforms1 = {1, 2};
        std::vector<int> platforms2 = {1, 2, 3};
        station1->addPlatforms(platforms1);
        station2->addPlatforms(platforms2);
        
        // Add lines to platforms
        auto* platform1 = station1->findPlatform(1);
        auto* platform2 = station2->findPlatform(1);
        assert(platform1 != nullptr && platform2 != nullptr);
        
        std::vector<int> lines1 = {1, 2};
        std::vector<int> lines2 = {3, 4, 5};
        platform1->addLines(lines1);
        platform2->addLines(lines2);
        
        // Test train schedules across stations
        station1->addTrainSchedule(1, 1, Time(10, 0), true);
        station1->addTrainSchedule(1, 2, Time(10, 30), false);
        station2->addTrainSchedule(1, 3, Time(10, 0), true);
        station2->addTrainSchedule(1, 4, Time(10, 30), false);
        
        // Test non-existent station
        assert(railway.findStation("S3") == nullptr);
        
        // Test display functionality (visual inspection required)
        railway.displayAllStations();
    }

    void testEdgeCases() {
        std::cout << "Testing edge cases...\n";
        
        RailwayStation<std::string> station("S1", "Central");
        
        // Test empty platform numbers
        try {
            std::vector<int> emptyPlatforms;
            station.addPlatforms(emptyPlatforms);
            assert(false && "Should throw exception for empty platform list");
        } catch (const RailwayException&) {}
        
        // Test platform with zero or negative numbers
        try {
            station.addPlatform(0);
            assert(false && "Should throw exception for invalid platform number");
        } catch (const RailwayException&) {}
        
        try {
            station.addPlatform(-1);
            assert(false && "Should throw exception for negative platform number");
        } catch (const RailwayException&) {}
        
        // Add a valid platform and test line edge cases
        station.addPlatform(1);
        auto* platform = station.findPlatform(1);
        assert(platform != nullptr);
        
        try {
            std::vector<int> emptyLines;
            platform->addLines(emptyLines);
            assert(false && "Should throw exception for empty line list");
        } catch (const RailwayException&) {}
        
        try {
            platform->addLine(0);
            assert(false && "Should throw exception for invalid line number");
        } catch (const RailwayException&) {}
        
        // Test time edge cases
        try {
            Time invalidTime(-1, 0);
            assert(false && "Should throw exception for negative hours");
        } catch (const RailwayException&) {}
        
        try {
            Time invalidTime(0, -1);
            assert(false && "Should throw exception for negative minutes");
        } catch (const RailwayException&) {}
    }

public:
    void runAllTests() {
        std::cout << "Running railway system tests...\n\n";
        
        testTimeClass();
        testPlatformAndLine();
        testRailwayStation();
        testRailwaySystem();
        testEdgeCases();
        
        std::cout << "\nAll tests passed successfully!\n";
    }
};

int main() {
    TestRailwaySystem tests;
    try {
        tests.runAllTests();
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
