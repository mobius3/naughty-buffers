cmake_minimum_required(VERSION 3.26)
project(naughty-buffers LANGUAGES C VERSION 1.0.0)
include(CTest)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

add_subdirectory(src)

if (BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

include(cmake/setup-exports.cmake)
include(cmake/naughty-buffers-cpack.cmake)
