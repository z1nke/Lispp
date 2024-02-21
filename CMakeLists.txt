cmake_minimum_required(VERSION 3.14)

project(lispp
  VERSION 0.1.0
  DESCRIPTION "A lisp interpreter in C++"
  LANGUAGES CXX
)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Debug.")
  set(CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")

# Generate a CompilationDatabase (compile_commands.json file) for our build,
# for use by clang_complete, YouCompleteMe, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lispp
  lisp.cpp
)
