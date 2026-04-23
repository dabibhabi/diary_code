# -------- Config --------
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS  ?= -pthread

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/task_thread

# Every class lives in its own folder under src/. Add each to -I so short
# includes (#include "task.hpp") keep working.
INCLUDES := -Isrc -Isrc/Task -Isrc/Worker -Isrc/Pool -Isrc/Runner

SRCS := main.cpp \
        src/Task/task.cpp \
        src/Worker/worker.cpp \
        src/Pool/pool.cpp \
        src/Runner/runner.cpp

# Object files mirror source layout under build/, e.g. build/src/Task/task.o
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# -------- Targets --------
.PHONY: all build run clean help
.DEFAULT_GOAL := build

all: build

## build : compile everything into $(TARGET)
build: $(TARGET)

## run   : build then execute the binary
run: build
	@echo "--- running $(TARGET) ---"
	@./$(TARGET)

## clean : remove the build directory (only if it exists)
clean:
	@if [ -d $(BUILD_DIR) ]; then \
		rm -rf $(BUILD_DIR); \
		echo "cleaned $(BUILD_DIR)/"; \
	else \
		echo "nothing to clean — $(BUILD_DIR)/ does not exist"; \
	fi

help:
	@grep -E '^##' $(MAKEFILE_LIST) | sed 's/## //'

# -------- Rules --------
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# -MMD -MP emits a .d file alongside each .o so header changes trigger rebuilds.
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(DEPS)
