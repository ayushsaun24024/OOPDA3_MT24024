CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
DEBUG_FLAGS = -g -O0
COMPILE_FLAGS = -c
RELEASE_FLAGS = -O3

MAIN_SRC = main.cpp
TEST_SRC = railway_tests.cpp
HEADERS = railway.h

DEBUG_TARGET = railway_debug
RELEASE_TARGET = railway_release
TEST_TARGET = railway_tests

all: optimize debug release tests

optimize: $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(COMPILE_FLAGS) $(MAIN_SRC)

debug: $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(MAIN_SRC) -o $(DEBUG_TARGET)

release: $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) $(MAIN_SRC) -o $(RELEASE_TARGET)

tests: $(TEST_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(TEST_SRC) -o $(TEST_TARGET)

clean:
	rm -f $(DEBUG_TARGET) $(RELEASE_TARGET) $(TEST_TARGET)

.PHONY: all debug release tests clean
