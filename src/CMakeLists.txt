list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake-modules)

add_subdirectory(naughty-buffers)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()
