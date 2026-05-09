# 20 — Build System

CMake drives the C++ build; **scikit-build-core** drives the Python wheel; nanobind does the binding glue. A thin top-level `Makefile` provides the day-to-day entry points; everything underneath is CMake.

## Daily commands (Makefile entry points)

```
make build    # plain Debug build (no sanitizers); honors BUILD_DIR + BUILD_TYPE
make dev      # CMakePresets dev preset — Debug + ASan + UBSan
make test     # dev build + ctest --output-on-failure
make run      # plain build + run the cuny_sms_demo binary
make tidy     # clang-tidy across all sources (against the dev compile_commands.json)
make format   # clang-format across all sources
make clean    # nuke build/ and dist/
make help     # quick listing
```

`make test` is the most common — it does a full configure + build under the dev preset (sanitizers active), then runs the whole Catch2 suite. The Makefile is intentionally thin: every target shells out to `cmake` or `ctest`, so you can drop down to those directly when you need to.

## Layout

```
cpp_test/
├── README.md                       # entry point: status, quick start, layout
├── DESIGN.md                       # design index → docs/
├── Makefile                        # convenience over CMake
├── CMakeLists.txt                  # top-level
├── CMakePresets.json               # dev / release / tsan / python presets
├── pyproject.toml                  # wheel build via scikit-build-core (Phase 7)
├── main.cpp                        # cuny_sms_demo binary
├── .clang-format
├── .clang-tidy
├── .gitignore
├── .pre-commit-config.yaml
├── include/cuny_sms/               # headers organized by design-pattern category
│   ├── identity.hpp                # umbrella
│   ├── domain/                     # value types (mostly struct keyword)
│   ├── factories/                  # Factory + Abstract Factory + Facade
│   ├── graph/                      # graph data structures
│   ├── persistence/                # loaders + repositories
│   └── strategies/                 # Strategy pattern (Phase 4+)
├── src/                            # implementation files (header-only for now)
├── tests/                          # Catch2 v3 unit tests
├── bindings/                       # nanobind module entry (Phase 7)
├── python/cuny_sms/                # Python package skeleton
├── data/
│   ├── catalogs/                   # 12 TSV files of real CUNY courses
│   └── test_names/                 # 500 first + 500 last names
└── docs/                           # design chapters (28 files)
```

## CMakeLists.txt — top level

```cmake
cmake_minimum_required(VERSION 3.20)
project(cuny_sms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CUNY_BUILD_TESTS "Build unit tests" ON)
option(CUNY_BUILD_PYTHON "Build the nanobind module" OFF)
option(CUNY_ENABLE_SANITIZERS "Enable ASan + UBSan in Debug" OFF)

add_library(cuny_sms_core STATIC
    src/identity/email_generator.cpp
    src/identity/cuny_member.cpp
    src/role/role_factory_registry.cpp
    src/domain/prerequisite_graph.cpp
    src/services/enrollment_service.cpp
    src/services/attendance_service.cpp
    src/services/waitlist_manager.cpp
    src/persistence/in_memory_database.cpp
)
target_include_directories(cuny_sms_core PUBLIC include)
target_compile_features(cuny_sms_core PUBLIC cxx_std_20)
target_compile_options(cuny_sms_core PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O2>
)

if(CUNY_ENABLE_SANITIZERS)
    target_compile_options(cuny_sms_core PUBLIC
        -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(cuny_sms_core PUBLIC -fsanitize=address,undefined)
endif()

if(CUNY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CUNY_BUILD_PYTHON)
    add_subdirectory(bindings)
endif()
```

The `cuny_sms_core` library is the single artifact everything else links against. Tests link it directly; the nanobind module links it as well.

---

## CMakePresets.json — sanity for everyone

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "dev",
      "displayName": "Debug + sanitizers",
      "binaryDir": "${sourceDir}/build/dev",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CUNY_BUILD_TESTS": "ON",
        "CUNY_ENABLE_SANITIZERS": "ON"
      }
    },
    {
      "name": "release",
      "displayName": "Release",
      "binaryDir": "${sourceDir}/build/release",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CUNY_BUILD_TESTS": "ON"
      }
    },
    {
      "name": "tsan",
      "displayName": "ThreadSanitizer",
      "binaryDir": "${sourceDir}/build/tsan",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "RelWithDebInfo",
        "CMAKE_CXX_FLAGS": "-fsanitize=thread -fno-omit-frame-pointer",
        "CMAKE_EXE_LINKER_FLAGS": "-fsanitize=thread"
      }
    }
  ],
  "buildPresets": [
    { "name": "dev",     "configurePreset": "dev" },
    { "name": "release", "configurePreset": "release" },
    { "name": "tsan",    "configurePreset": "tsan" }
  ]
}
```

Day-to-day commands collapse to:

```bash
cmake --preset dev && cmake --build --preset dev && ctest --test-dir build/dev
```

---

## pyproject.toml — wheel build via scikit-build-core

```toml
[build-system]
requires = ["scikit-build-core>=0.10", "nanobind>=2.0"]
build-backend = "scikit_build_core.build"

[project]
name = "cuny_sms"
version = "0.0.1"
description = "CUNY Student Management System — C++/Python interop demo"
requires-python = ">=3.11"
dependencies = []

[project.optional-dependencies]
dev = ["pytest", "pytest-cov", "hypothesis", "ruff", "mypy"]

[tool.scikit-build]
cmake.version = ">=3.20"
cmake.args = ["-DCUNY_BUILD_PYTHON=ON", "-DCUNY_BUILD_TESTS=OFF"]
wheel.packages = ["python/cuny_sms"]
wheel.expand-macos-universal-tags = false

[tool.scikit-build.cmake.define]
CUNY_BUILD_PYTHON = "ON"
```

`pip install -e .` builds the C++ library, the nanobind module, links them, drops the `.so` into `python/cuny_sms/_core...so`, and installs the package editable.

---

## bindings/CMakeLists.txt

```cmake
find_package(nanobind CONFIG REQUIRED)
find_package(Python 3.11 COMPONENTS Interpreter Development.Module REQUIRED)

nanobind_add_module(_core STABLE_ABI _core.cpp)
target_link_libraries(_core PRIVATE cuny_sms_core)
install(TARGETS _core LIBRARY DESTINATION cuny_sms)
```

`STABLE_ABI` lets one wheel work across Python 3.11+ without recompiling per version.

---

## tests/CMakeLists.txt

```cmake
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests
    test_identity.cpp
    test_prereq_graph.cpp
    test_enrollment.cpp
    test_attendance.cpp
)
target_link_libraries(unit_tests PRIVATE cuny_sms_core Catch2::Catch2WithMain)
add_test(NAME unit_tests COMMAND unit_tests)
```

**Catch2 v3** for headers + main; matches modern C++ idioms; pretty test failure output.

---

## .pre-commit-config.yaml

```yaml
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v18.1.8
    hooks:
      - id: clang-format
        types_or: [c++, c]
  - repo: https://github.com/astral-sh/ruff-pre-commit
    rev: v0.6.4
    hooks:
      - id: ruff-format
      - id: ruff
        args: [--fix]
  - repo: https://github.com/pre-commit/pre-commit-hooks
    rev: v4.6.0
    hooks:
      - id: trailing-whitespace
      - id: end-of-file-fixer
      - id: check-yaml
      - id: check-added-large-files
```

Runs on every commit. Catches the obvious before CI does.

---

## .clang-format

```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
PointerAlignment: Left
AccessModifierOffset: -4
NamespaceIndentation: None
SortIncludes: CaseInsensitive
IncludeBlocks: Regroup
```

100-column matches Python. `PointerAlignment: Left` (`int* p` not `int *p`) is the modern norm.

---

## .clang-tidy

```yaml
Checks: >
  -*,
  bugprone-*,
  cppcoreguidelines-*,
  modernize-*,
  performance-*,
  readability-*,
  -modernize-use-trailing-return-type,
  -readability-identifier-length,
  -cppcoreguidelines-avoid-magic-numbers,
  -readability-magic-numbers,
WarningsAsErrors: '*'
```

Disables the noisier checks (trailing return types, identifier length, magic numbers — the project has plenty of meaningful constants like 9-digit emp IDs).

---

## CI sketch (out of scope for first build)

```
matrix:
  os: [ubuntu-latest, macos-latest]
  preset: [dev, release]
steps:
  - cmake --preset {{preset}}
  - cmake --build --preset {{preset}}
  - ctest --test-dir build/{{preset}}
  - pip install -e ".[dev]"
  - pytest python/tests
  - ruff check python/
  - mypy --strict python/
```

---

## Cross-references

- The C++ choices behind these flags — `18-cpp-style.md`
- Python tooling rationale — `19-python-style.md`
- nanobind module specifics — `interop/21-binding-strategy.md`
