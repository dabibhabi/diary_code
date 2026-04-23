CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC_HDRS = src/locatable.hpp src/point.hpp src/trail.hpp
TEST_SRC = tests/test_runner.cpp tests/test_point.cpp tests/test_locatable.cpp tests/test_trail.cpp

.PHONY: all run test clean

all: main test_runner

main: main.cpp $(SRC_HDRS)
	$(CXX) $(CXXFLAGS) main.cpp -o main

test_runner: $(TEST_SRC) tests/test_helper.hpp $(SRC_HDRS)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o test_runner

run: main
	./main

test: test_runner
	./test_runner

clean:
	rm -f main test_runner
