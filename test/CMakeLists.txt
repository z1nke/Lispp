function(add_test_target target)
  add_executable(${target} ${target}.cpp)
  target_link_libraries(${target}
    PRIVATE
    lisp
  )
  target_include_directories(${target}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
  )
  add_test(NAME ${target} COMMAND ${target})
endfunction ()

add_test_target(TestRead)
add_test_target(TestEval)
