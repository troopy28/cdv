# Add the test files.
file(GLOB_RECURSE CDV_TESTS_LIST CONFIGURE_DEPENDS "${cdv_SOURCE_DIR}/tests/*.cpp")
add_executable(cdv_tests ${CDV_TESTS_LIST} "${cdv_SOURCE_DIR}/.clang-format")

target_compile_features(cdv_tests PRIVATE cxx_std_17)

add_test(NAME cdv_tests COMMAND cdv_tests)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CDV_TESTS_LIST})
