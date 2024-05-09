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
  if (WIN32)
    add_test(NAME ${target} COMMAND ${target}.exe)
  else ()
    add_test(NAME ${target} COMMAND ${target})
  endif ()
endfunction ()

add_test_target(TestRead)
add_test_target(TestEval)
