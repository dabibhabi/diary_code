# Convenience targets over CMake (see CMakeLists.txt + CMakePresets.json).
#
# Usage:
#   make build         # configure + build (Debug, no sanitizers)
#   make dev           # configure + build via CMakePresets dev (ASan + UBSan)
#   make test          # build dev + run ctest
#   make run           # build and run the C++ demo binary
#   make tidy          # run clang-tidy across all sources
#   make format        # clang-format all sources
#   make clean         # remove all build trees
#   make help          # this help
#
# Knobs:
#   BUILD_DIR=build       # output directory for the plain `build` target
#   BUILD_TYPE=Debug      # CMake build type for the plain `build` target

BUILD_DIR  ?= build
BUILD_TYPE ?= Debug
CMAKE      ?= cmake
DEMO       := cuny_sms_demo
DEV_DIR    := build/dev

.PHONY: all build dev test run tidy format clean help

all: build

help:
	@echo "Targets: build, dev, test, run, tidy, format, clean"
	@echo "  BUILD_DIR=$(BUILD_DIR)  BUILD_TYPE=$(BUILD_TYPE)"

# ---------------------------------------------------------------------------
# Plain build — no presets, no sanitizers. Fast iteration.
# ---------------------------------------------------------------------------
build:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(CMAKE) --build $(BUILD_DIR) --parallel

# ---------------------------------------------------------------------------
# Sanitizer-enabled dev build via CMakePresets.json.
# ---------------------------------------------------------------------------
dev:
	$(CMAKE) --preset dev
	$(CMAKE) --build --preset dev

# ---------------------------------------------------------------------------
# Run the unit-test suite (built under the dev preset for full coverage).
# ---------------------------------------------------------------------------
test: dev
	ctest --test-dir $(DEV_DIR) --output-on-failure

run: build
	./$(BUILD_DIR)/$(DEMO)

# ---------------------------------------------------------------------------
# Static analysis. Requires clang-tidy + a compile_commands.json (produced by
# CMake configure). Won't fail on style nits — see .clang-tidy for what's
# treated as an error.
# ---------------------------------------------------------------------------
tidy: dev
	@find include tests bindings -name '*.hpp' -o -name '*.cpp' \
	  | xargs clang-tidy -p $(DEV_DIR) || true
	@clang-tidy -p $(DEV_DIR) main.cpp || true

format:
	@find include tests bindings -type f \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' \) -print0 \
	  | xargs -0 -r clang-format -i
	@clang-format -i main.cpp

clean:
	rm -rf build dist
