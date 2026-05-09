# nanobind module — populated in Phase 7 of docs/25-todo-roadmap.md.
#
# This file is intentionally minimal until then; uncomment when nanobind is in
# the build environment (scikit-build-core supplies it via pyproject.toml).

# find_package(nanobind CONFIG REQUIRED)
# find_package(Python 3.11 COMPONENTS Interpreter Development.Module REQUIRED)
#
# nanobind_add_module(_core STABLE_ABI _core.cpp)
# target_link_libraries(_core PRIVATE cuny_sms_core)
# install(TARGETS _core LIBRARY DESTINATION cuny_sms)
#
# # post-build stub generation
# add_custom_command(TARGET _core POST_BUILD
#     COMMAND ${Python_EXECUTABLE} -m nanobind.stubgen
#             --module cuny_sms._core
#             --output-file ${CMAKE_SOURCE_DIR}/python/cuny_sms/_core.pyi
#     BYPRODUCTS ${CMAKE_SOURCE_DIR}/python/cuny_sms/_core.pyi
#     COMMENT "Generating Python stubs"
# )
