# 18 — C++17/20 Style & Conventions

What "modern C++" means for this codebase. Not a comprehensive style guide — just the choices we've made and why.

## `struct` vs `class` — semantic signal, not just default access

We use the keyword as documentation:

| Keyword | When | Examples |
|---------|------|----------|
| `struct` | A small composite-component value type, used as a building block for bigger objects. Public-by-default keyword signals "this is meant to be composed". | `Name`, `Email`, `EmployeeId`, `Person`, `Campus`, `Course`, `Section`, `MeetingSlot`, `DaySet`, `CourseId` |
| `class` | A larger object that *composes* structs and owns behavior or invariants. | `CunyMember`, every factory, `CampusServices`, `PrerequisiteGraph`, `CatalogLoader`, `Role` and its polymorphic subclasses |

Mechanically a `struct` and a `class` differ only in default access and default inheritance visibility — both can have private members, validating constructors, and methods. The choice is a reader hint:

- See `struct X`? Expect a value type, probably composed into something larger.
- See `class Y`? Expect behavior, invariants, and likely composition of structs.

A `struct` may still hide its fields behind `private:` if it has invariants worth enforcing in the constructor (e.g., `Name` requires both names non-empty). The keyword is about *intent*, not field visibility.

---

## Pattern → directory mapping

Headers live under `include/cuny_sms/{domain, factories, graph, persistence, strategies}/`. The directory name is the design-pattern category:

| Directory | What goes here |
|-----------|----------------|
| `domain/` | Value types and enums — the building blocks (mostly `struct`s) |
| `factories/` | Creational patterns — Factory, Abstract Factory, Facade |
| `graph/` | Graph data structures (DAG, BFS layers, etc.) |
| `persistence/` | Loaders and (later) repositories |
| `strategies/` | Behavioral pattern — Strategy (Phase 4 onward) |

Find a class by its design pattern, not by its alphabetical position. New code goes in the directory matching its pattern.

---

## Standard

**C++20** is the target. Compilers: GCC ≥ 11, Clang ≥ 14, MSVC ≥ 19.30. CMake declares it via:

```cmake
target_compile_features(cuny_sms_core PUBLIC cxx_std_20)
```

We do not chase C++23 yet — `std::expected` is the main thing missing, and `tl::expected` covers it.

---

## C++17 features we lean on

| Feature | Where |
|---------|-------|
| `std::optional<T>` | "Maybe a member" returns; Phase 5 attendance lookups |
| `std::variant<T...>` | Status enums when they need richer payloads |
| `std::string_view` | All read-only string params; never own a copy you don't need |
| Structured bindings | `auto [it, inserted] = map.emplace(...)` |
| `if constexpr` | Compile-time branching in templates (concept-lite) |
| `[[nodiscard]]` | Every "computed value" return — caller must use it |
| `std::filesystem` | Config loading, log output paths |
| Class template argument deduction | `std::pair{a, b}` instead of `std::pair<int, double>{a, b}` |

---

## C++20 features we lean on

### Concepts

Constrain templates to spell out interfaces:

```cpp
template <typename F>
concept RoleFactoryLike = requires(F f, RoleData d) {
    { f.createStudentRole(d) } -> std::same_as<StudentRole>;
    { f.createAdminRole(d) }   -> std::same_as<AdminRole>;
    { f.validateRoleData(RoleType::Student, d) } -> std::convertible_to<bool>;
};
```

Concepts replace SFINAE for 95% of cases — error messages drop from screenfuls to a single line.

### Ranges

Pipeline-style algorithms:

```cpp
auto active = enrollments
            | std::views::filter([](const auto& e){ return e.status == Status::Enrolled; })
            | std::views::transform(&Enrollment::courseId);
```

Beats hand-rolled loops for read-only chains. Avoid for in-place mutation; classic `std::for_each` is clearer there.

### `std::span<T>`

Non-owning view over a contiguous buffer. Critical for crossing the nanobind boundary without copy:

```cpp
double meanAttendance(std::span<const double> rates);
// nanobind binds std::span to numpy arrays zero-copy.
```

Replaces "pointer + length" pairs everywhere.

### Three-way comparison `<=>`

Get all six relational operators from one default:

```cpp
struct WaitlistEntry {
    int priority;
    std::int64_t queuedAtNs;
    auto operator<=>(const WaitlistEntry&) const = default;
};
```

For `std::priority_queue`-friendly types this collapses ~30 lines of operator boilerplate.

### `std::format`

Type-safe formatting — replaces `<<` chains and `printf`:

```cpp
std::string s = std::format("Enrolled {} in {}", member.email(), course.name());
```

If your toolchain lacks `<format>` (older libc++), use `fmt::format` from {fmt}. Same syntax, header-only.

### Designated initializers

```cpp
MeetingSlot mws{ .days = Days::MWF, .startMinute = 600, .endMinute = 660 };
```

Documents which field is which at the call site.

---

## Resource management

**Default: rule of zero.** Most types should rely on the compiler-generated copy/move/destructor. Only when you own a raw resource (`FILE*`, OS handle, custom allocation) implement the rule of five — and then prefer to wrap the resource in a `unique_ptr<T, CustomDeleter>` and *still* use rule of zero.

**Smart pointers.**

| Type | When |
|------|------|
| `std::unique_ptr<T>` | Sole ownership. Default for owned objects. |
| `std::shared_ptr<T>` | Genuinely shared. `Role` shared between `CunyMember` and section roster. Justify each use. |
| `std::weak_ptr<T>` | Break cycles. `Course` → `Department` → `Course` etc. |
| Raw pointer | Non-owning. Always non-null in our code; use a reference if you can. |

**Move semantics.** Pass by value when you'll consume; `&&` for sink parameters; `const T&` for read-only access to expensive types; `T` for cheap (`int`, `double`, `string_view`).

```cpp
void EnrollmentService::enroll(Enrollment e);              // takes ownership
void EnrollmentService::quote(const Enrollment& e) const;  // read-only
```

---

## `constexpr` and `consteval`

Course IDs, schedule slot constants, prereq edges in the seed catalog — all `constexpr`. For things that **must** be compile-time (e.g., catalog code validators), use `consteval`:

```cpp
consteval Department department_from(std::string_view code) {
    if (code == "MATH")  return Department::Math;
    if (code == "CS")    return Department::CS;
    if (code == "STATS") return Department::Stats;
    if (code == "PHYS")  return Department::Physics;
    throw "unknown code";   // throws at compile time -> compile error
}
```

This pushes "is this a real department?" into the compile step.

---

## Error handling

Two channels:

| Kind | Mechanism | Example |
|------|-----------|---------|
| Invariant violation (programmer error) | Exception | `PrereqCycleError` on `addPrereq` cycle |
| Predictable failure (data tells us no) | `tl::expected<T, E>` | "Enrollment rejected: missing prereq CS201" |

`tl::expected` (or C++23 `std::expected`) is a sum type carrying either a result or an error code. Functions that can fail in normal operation return one:

```cpp
tl::expected<Enrollment, EnrollmentRejection>
EnrollmentService::enroll(MemberId student, CourseId course);
```

Callers see the error path in the type. No surprise exceptions on the hot path.

---

## Build & QA

| Tool | Purpose | Setting |
|------|---------|---------|
| CMake (≥ 3.20) | Build | `cmake_minimum_required(VERSION 3.20)` |
| Ninja | Generator | `cmake -G Ninja` (faster than Make) |
| Sanitizers | Runtime checking | ASan + UBSan in dev preset; TSan in a separate preset |
| `clang-tidy` | Static analysis | `-checks=bugprone-*,cppcoreguidelines-*,modernize-*,performance-*,readability-*` |
| `clang-format` | Format | LLVM style, 100-col, sorted includes |
| Compiler flags | Warnings as errors | `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror` |

Sanitizers in `CMakePresets.json`:

```json
{
  "name": "dev-asan",
  "cacheVariables": {
    "CMAKE_BUILD_TYPE": "Debug",
    "CMAKE_CXX_FLAGS": "-fsanitize=address,undefined -fno-omit-frame-pointer"
  }
}
```

---

## Existing `main.cpp` — issues to fix in Phase 1

The current starter has bugs the conventions in this chapter would catch:

1. **Missing `;` after `Emplid` struct closing brace** — compile error in stricter modes.
2. **`std::rand()` in `Emplid` constructor** — low-quality PRNG, not thread-safe, and *regenerates the ID every call*. Replace with `<random>`:
   ```cpp
   class Emplid {
       int value_;
   public:
       Emplid() {
           static thread_local std::mt19937 rng{std::random_device{}()};
           std::uniform_int_distribution<int> dist(100'000'000, 999'999'999);
           value_ = dist(rng);
       }
       [[nodiscard]] int value() const noexcept { return value_; }
   };
   ```
3. **`getEmail()` re-derives email each call** — should compute once in the constructor and store.
4. **Headers concentrated in `main.cpp`** — split into `include/cuny_sms/{name,email,emplid,student}.hpp` with implementation in `src/`.

These land in Phase 1 of the roadmap (`25-todo-roadmap.md`).

---

## Cross-references

- Build system + presets — `20-build-system.md`
- Concept-constrained `RoleFactory` — `04-patterns.md`
- Where `std::span` crosses to Python — `interop/21-binding-strategy.md`
