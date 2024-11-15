# Railway Management System

## Overview
The Railway Management System is a C++ application that helps manage railway stations, platforms, train lines, and schedules. It provides a hierarchical structure where stations contain platforms, platforms contain lines, and lines contain train schedules.

## Table of Contents
- [System Architecture](#system-architecture)
- [Key Components](#key-components)
- [Class Hierarchy](#class-hierarchy)
- [Building and Running](#building-and-running)
- [Usage Guide](#usage-guide)
- [Features and Constraints](#features-and-constraints)
- [Testing](#testing)
- [Error Handling](#error-handling)

## System Architecture

The system follows a hierarchical structure:
```
RailwaySystem
└── RailwayStation
    └── Platform
        └── Line
            └── TrainSchedule
```

## Key Components

### 1. Time Class
- Handles time representation in 24-hour format (HH:MM)
- Validates hours (0-23) and minutes (0-59)
- Provides comparison operators and time difference calculations
- Formats time as string (e.g., "14:30")

### 2. TrainSchedule Class
- Stores schedule information: time and train type (stopping/through)
- Train types:
  - Stopping trains: Stop at all stations
  - Through trains: Pass through without stopping

### 3. Line Class
- Manages train schedules for a specific line
- Enforces scheduling rules:
  - 30-minute minimum gap between stopping trains
  - 10-minute minimum gap between through trains
- Validates schedule conflicts

### 4. Platform Class
- Contains multiple lines
- Manages line addition and retrieval
- Ensures unique line numbers within platform

### 5. RailwayStation Class (Template)
- Generic station identifier (supports different ID types)
- Manages multiple platforms
- Handles platform and line operations
- Coordinates train schedule additions

### 6. RailwaySystem Class
- Top-level system management
- Handles station addition and lookup
- Provides system-wide display functionality

## Class Hierarchy

Detailed class relationships and key methods:

```
RailwaySystem<T>
├── addStation(id: T, name: string)
├── findStation(id: T)
└── displayAllStations()

RailwayStation<T>
├── addPlatform(number: int)
├── addPlatforms(numbers: vector<int>)
├── findPlatform(number: int)
└── addTrainSchedule(platform: int, line: string, time: Time, isStoppingTrain: bool)

Platform
├── addLine(number: string)
├── addLines(numbers: vector<string>)
└── findLine(number: string)

Line
├── addTrain(time: Time, isStoppingTrain: bool)
└── canAddTrain(time: Time, isStoppingTrain: bool)
```

## Building and Running

### Prerequisites
- C++17 compatible compiler (g++)
- Make build system

### Build Commands
```bash
# Build all targets
make all

# Build debug version
make debug

# Build optimized release version
make release

# Build tests
make tests

# Clean build files
make clean
```

### Running the Application
```bash
# Run debug version
./railway_debug

# Run release version
./railway_release

# Run tests
./railway_tests
```

## Usage Guide

### Main Menu Options
1. Add a station
   - Enter station ID and name
   - Station names can contain multiple words

2. Add platforms to station
   - Enter station ID
   - Enter space-separated platform numbers

3. Add lines to platform
   - Enter station ID and platform number
   - Enter space-separated line identifiers

4. Add train schedule
   - Enter station ID, platform number, and line identifier
   - Enter time (hours and minutes)
   - Specify train type (S for Stopping, T for Through)

5. View entire system
   - Displays complete system hierarchy and schedules

6. Exit program

## Features and Constraints

### Time Constraints
- Stopping trains: 30-minute minimum gap
- Through trains: 10-minute minimum gap
- Valid time format: 00:00 to 23:59

### Validation Rules
- Positive platform numbers only
- Unique station IDs
- Unique platform numbers per station
- Unique line identifiers per platform
- Valid time slots based on train type

## Testing

The system includes comprehensive unit tests (`railway_tests.cpp`) covering:
- Time operations and validation
- Platform and line management
- Station operations
- Schedule conflicts
- Edge cases and error conditions

### Test Categories
1. Time Class Tests
2. Platform and Line Tests
3. Railway Station Tests
4. Railway System Tests
5. Edge Case Tests

## Error Handling

The system uses custom exceptions for error handling:

### RailwayException
Base exception class for system-wide errors:
- Invalid input validation
- Duplicate entity creation
- Entity not found
- Invalid parameters

### TimeConflictException
Specific exception for schedule conflicts:
- Insufficient time gap between trains
- Overlapping schedules

### Error Recovery
- All exceptions are caught and handled gracefully
- User-friendly error messages displayed
- System state remains consistent after errors

## Implementation Notes

1. STL Usage
   - `vector` for collections
   - `unique_ptr` for memory management
   - `string` for text handling
   - `stringstream` for input parsing

2. Memory Management
   - RAII principles followed
   - Smart pointers prevent memory leaks
   - Copy operations prevented where appropriate

3. Template Usage
   - Station IDs can be of any type
   - Type safety enforced at compile time

4. Input Validation
   - All user input validated
   - Buffer overflow protection
   - Type checking and conversion handling
   