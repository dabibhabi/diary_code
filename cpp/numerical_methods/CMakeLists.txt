cmake_minimum_required(VERSION 3.18)
project(numerical_methods CXX C)

option(NM_USE_CUDA   "Build CUDA challenge kernels"     OFF)
option(NM_USE_PYTHON "Build the pybind11 Python module" OFF)
option(NM_BUILD_FFT  "Build the (stretch) FFT TU"       ON)
option(NM_USE_SANITIZERS "Build with -fsanitize=address,undefined (Debug)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

if(NM_USE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# ---- Pure-C primitives (callable from C, C++, and CUDA) -------------------
file(GLOB NM_C_SOURCES CONFIGURE_DEPENDS c/*.c)
list(REMOVE_ITEM NM_C_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/c/test_c_api.c)
add_library(nm_c_core STATIC ${NM_C_SOURCES})
target_include_directories(nm_c_core PUBLIC include)

# Standalone C executable — built with C compiler, no C++ runtime.
add_executable(test_c_api_pure c/test_c_api.c)
target_link_libraries(test_c_api_pure PRIVATE nm_c_core)
set_target_properties(test_c_api_pure PROPERTIES LINKER_LANGUAGE C)

# ---- C++ core library -----------------------------------------------------
file(GLOB_RECURSE NM_CXX_SOURCES CONFIGURE_DEPENDS src/*.cpp)
if(NOT NM_BUILD_FFT)
    list(REMOVE_ITEM NM_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/dsa/fft.cpp)
endif()
add_library(nm_core ${NM_CXX_SOURCES})
target_include_directories(nm_core PUBLIC include)
target_link_libraries(nm_core PUBLIC nm_c_core)

# ---- Runner ---------------------------------------------------------------
add_executable(numerical_methods main.cpp)
target_link_libraries(numerical_methods PRIVATE nm_core)

# ---- Tests with TODO-slug LABELS -----------------------------------------
enable_testing()

# Map test-name → label(s). The first label matches a TODO slug used by
# scripts/parse_todo.py; "baseline" makes a test always run.
function(nm_add_labeled_test test_src)
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE nm_core)
    target_include_directories(${test_name} PRIVATE tests)
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "${ARGN}")
endfunction()

# Always-on (baseline) tests
nm_add_labeled_test(tests/test_utils.cpp                      baseline)
nm_add_labeled_test(tests/test_observer.cpp                   baseline)
nm_add_labeled_test(tests/test_not_implemented_through_vtable.cpp baseline)

# Slug-gated tests — labels match TODO.md `- [x] **<slug>**` entries.
nm_add_labeled_test(tests/test_bisection.cpp                  bisection)
nm_add_labeled_test(tests/test_newton.cpp                     newton)
nm_add_labeled_test(tests/test_secant.cpp                     secant)
nm_add_labeled_test(tests/test_brent.cpp                      brent)
nm_add_labeled_test(tests/test_root_finders_extra.cpp         halley ridders muller chandrupatla)
nm_add_labeled_test(tests/test_iterative_method.cpp           iterative_root_finder)
nm_add_labeled_test(tests/test_factory.cpp                    solver_factory)
nm_add_labeled_test(tests/test_mc_builder.cpp                 mc_engine_builder)
nm_add_labeled_test(tests/test_decorators.cpp                 antithetic_decorator control_variate_decorator)
nm_add_labeled_test(tests/test_c_api.cpp                      xorshift inv_normal_cdf blas1 cholesky_inplace tridiag)
nm_add_labeled_test(tests/test_crtp_dispatch.cpp              crtp_bisection)
nm_add_labeled_test(tests/test_error_heap.cpp                 error_heap)
nm_add_labeled_test(tests/test_tridiag.cpp                    tridiag_struct)
nm_add_labeled_test(tests/test_csr.cpp                        csr_sparse)
nm_add_labeled_test(tests/test_fft.cpp                        fft bit_reverse)

# Pre-existing per-category tests (still slug-gated).
nm_add_labeled_test(tests/test_quadrature.cpp                 romberg adaptive_simpson gauss_legendre gauss_hermite tanh_sinh)
nm_add_labeled_test(tests/test_differentiation.cpp            richardson_derivative complex_step_derivative)
nm_add_labeled_test(tests/test_ode.cpp                        euler rk4 rk45)
nm_add_labeled_test(tests/test_linalg.cpp                     lu_decompose cholesky qr_decompose conjugate_gradient)
nm_add_labeled_test(tests/test_optimization.cpp               golden_section brent_min nelder_mead levenberg_marquardt)
nm_add_labeled_test(tests/test_pde.cpp                        heat_explicit heat_implicit crank_nicolson)
nm_add_labeled_test(tests/test_interpolation.cpp              natural_cubic_spline monotone_cubic pchip)
nm_add_labeled_test(tests/test_random_mc.cpp                  box_muller inv_normal_cdf sobol_1d)

# Standalone C executable as a ctest.
add_test(NAME test_c_api_pure COMMAND test_c_api_pure)
set_tests_properties(test_c_api_pure PROPERTIES LABELS "baseline")

# ---- CUDA (optional) -----------------------------------------------------
if(NM_USE_CUDA)
    include(CheckLanguage)
    check_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        enable_language(CUDA)
        set(CMAKE_CUDA_STANDARD 17)
        file(GLOB NM_CUDA_SOURCES CONFIGURE_DEPENDS cuda/*.cu)
        if(NM_CUDA_SOURCES)
            add_library(nm_cuda STATIC ${NM_CUDA_SOURCES})
            target_include_directories(nm_cuda PUBLIC include)
            set_target_properties(nm_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
            target_link_libraries(nm_core PUBLIC nm_cuda)
            message(STATUS "CUDA enabled — building kernels in cuda/")
        endif()
    else()
        message(WARNING "NM_USE_CUDA=ON but no CUDA compiler found — skipping CUDA targets")
    endif()
endif()

# ---- Python bindings (optional) ------------------------------------------
if(NM_USE_PYTHON)
    find_package(pybind11 CONFIG)
    if(pybind11_FOUND)
        pybind11_add_module(nmpy python/bindings.cpp)
        target_link_libraries(nmpy PRIVATE nm_core)
        message(STATUS "Python module 'nmpy' will be built")
    else()
        message(WARNING "NM_USE_PYTHON=ON but pybind11 not found — skipping module")
    endif()
endif()
