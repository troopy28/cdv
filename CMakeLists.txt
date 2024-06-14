cmake_minimum_required(VERSION 3.1)

project(
  cdv
  VERSION 0.1
  DESCRIPTION "C++ Data structure Visualization"
  LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)                   # Ensure -std=c++xx instead of -std=g++xx.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # Support folders in IDEs.
  
# Library itself.
add_library(cdv STATIC "${cdv_SOURCE_DIR}/include/cdv/cdv.hpp" "${cdv_SOURCE_DIR}/.clang-format")
set_target_properties(cdv PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(cdv PUBLIC cxx_std_17)
source_group(TREE "${cdv_SOURCE_DIR}" FILES "${cdv_SOURCE_DIR}/cdv/include/cdv.hpp")

# Tests.
enable_testing()
add_subdirectory(tests)                         # Code for unit tests.
