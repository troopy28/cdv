cmake_minimum_required(VERSION 3.1)

project(
  cdv
  VERSION 0.1
  DESCRIPTION "C++ Data structure Visualization"
  LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)                   # Ensure -std=c++xx instead of -std=g++xx.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # Support folders in IDEs.
  
add_subdirectory(cdv)

enable_testing()
add_subdirectory(tests)                         # Code for unit tests.
