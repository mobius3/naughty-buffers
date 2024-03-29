macro(nb_test)
  # multiple sources
  if (${ARGC} GREATER 1)
    set(test_name "${ARGV0}")
    set(src ${ARGN})
    list(REMOVE_AT src 0)
  else ()
    set(test_name "test-${ARGV0}")
    set(src "${ARGV0}.c")
  endif ()
  message(STATUS "Adding test ${test_name} (${src})")
  add_executable(${test_name} ${src})
  target_link_libraries(${test_name} naughty-buffers)
  if (MSVC)
    add_custom_command(TARGET ${test_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${test_name}> $<TARGET_FILE_DIR:${test_name}>
            COMMAND_EXPAND_LISTS
    )
  endif()
  add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

nb_test(test-memory-calls memory.c)
nb_test(test-push push.c)
nb_test(test-assign assign.c)
nb_test(test-assign-many assign-many.c)
nb_test(test-insert insert.c)
nb_test(test-remove remove.c)
nb_test(test-sort sort.c)
nb_test(test-array-generator array-generator.c)
nb_test(test-iterators iterators.c)
