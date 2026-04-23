SOURCES := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
HEADERS := $(wildcard include/Vec3/*.hpp) $(wildcard include/Vec3/*/*.hpp)

.PHONY: all format build run clean

all: format build run

format:
	clang-format -style=file -i $(SOURCES) $(HEADERS)

build: format
	cmake -S . -B build
	cmake --build build

run: build
	./build/vec3

clean:
	rm -rf build
