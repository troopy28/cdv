cmake_minimum_required(VERSION 3.1)

project(
  cdv
  VERSION 0.1
  DESCRIPTION "C++ Data structure Visualization"
  LANGUAGES CXX)

add_library(cdv STATIC "include/cdv.hpp")
set_target_properties(cdv PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(cdv PUBLIC cxx_std_17)
