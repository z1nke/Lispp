find_package(GTest REQUIRED)
include(GoogleTest)

function(add_test_target target)
  add_executable(${target} ${target}.cpp)
  target_link_libraries(${target}
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    lisp
  )
  target_include_directories(${target}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
  )
  gtest_discover_tests(${target})
endfunction ()

add_test_target(TestRead)
add_test_target(TestEval)
