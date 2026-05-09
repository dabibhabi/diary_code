include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests
    test_smoke.cpp
    test_identity.cpp
    test_catalog_loader.cpp
    test_prerequisite_graph.cpp
)
target_link_libraries(unit_tests PRIVATE
    cuny_sms_core
    cuny_sms_warnings
    Catch2::Catch2WithMain
)
target_compile_definitions(unit_tests PRIVATE
    CUNY_CATALOG_DIR="${CMAKE_SOURCE_DIR}/data/catalogs"
)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
