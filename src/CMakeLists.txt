list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake-modules)

add_subdirectory(naughty-buffers)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif ()

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()