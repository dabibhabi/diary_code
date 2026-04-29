BUILD_DIR ?= build
BIN       := $(BUILD_DIR)/stats_demo
JOBS      ?= $(shell nproc 2>/dev/null || echo 4)

CMAKE         ?= cmake
CLANG_FORMAT  ?= clang-format

SRC := $(shell find src include apps -type f \( -name '*.cpp' -o -name '*.hpp' \))

.PHONY: all build run clean format format-check help

all: build

build: $(BUILD_DIR)/CMakeCache.txt
	$(CMAKE) --build $(BUILD_DIR) -j $(JOBS)

$(BUILD_DIR)/CMakeCache.txt:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

run: build
	$(BIN)

clean:
	rm -rf $(BUILD_DIR)

format:
	$(CLANG_FORMAT) -i --style=file $(SRC)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror --style=file $(SRC)

help:
	@echo "Targets:"
	@echo "  build         configure (if needed) and compile"
	@echo "  run           build then run $(BIN)"
	@echo "  clean         remove $(BUILD_DIR)"
	@echo "  format        run clang-format -i on src/ and include/ using .clang-format"
	@echo "  format-check  fail if any file would be reformatted"
