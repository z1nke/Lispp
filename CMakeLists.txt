cmake_minimum_required(VERSION 3.20)

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

add_library(lisp
  lisp.cpp
)

add_executable(lispp
  main.cpp
)

target_link_libraries(lispp
  PRIVATE
  lisp
)

if (MSVC)
  target_compile_options(lispp PRIVATE /W4 /WX)
else ()
  target_compile_options(lispp PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()


option(LISPP_ENABLE_UNIT_TESTS "Unit tests" ON)
if (LISPP_ENABLE_UNIT_TESTS)
  enable_testing()
  set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
  add_subdirectory(test)
endif ()