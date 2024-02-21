cmake_minimum_required(VERSION 3.26)
project(naughty-buffers LANGUAGES C VERSION 1.1.1)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(
    CMAKE_C_STANDARD 99
    CMAKE_POSITION_INDEPENDENT_CODE ON
    CMAKE_C_EXTENSIONS OFF
    CMAKE_CXX_EXTENSIONS OFF
    CMAKE_C_VISIBILITY_PRESET hidden
    CMAKE_CXX_VISIBILITY_PRESET hidden
)

set(NAUGHTY_BUFFERS_PUBLIC_HEADERS
    include/naughty-buffers/buffer.h
    include/naughty-buffers/array-generator.h
    ${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers/naughty-buffers-export.h
)

add_library(naughty-buffers-objects OBJECT
    src/naughty-buffers/buffer.c
    src/naughty-buffers/memory.h
    src/naughty-buffers/memory.c
    ${NAUGHTY_BUFFERS_PUBLIC_HEADERS}
)

target_include_directories(naughty-buffers-objects PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_property(TARGET naughty-buffers-objects PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared version
add_library(naughty-buffers SHARED $<TARGET_OBJECTS:naughty-buffers-objects>)
add_library(naughty-buffers::naughty-buffers ALIAS naughty-buffers)

generate_export_header(naughty-buffers
    BASE_NAME naughty-buffers
    EXPORT_FILE_NAME naughty-buffers/naughty-buffers-export.h
)

set_target_properties(naughty-buffers PROPERTIES
    VERSION ${naughty-buffers_VERSION}
    SO_VERSION ${naughty-buffers_VERSION_MAJOR}
    PUBLIC_HEADER "${NAUGHTY_BUFFERS_PUBLIC_HEADERS}"
    OUTPUT_NAME "naughty-buffers"
    RUNTIME_OUTPUT_NAME naughty-buffers-${naughty-buffers_VERSION}
)

target_include_directories(naughty-buffers PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# static vertsion

add_library(naughty-buffers-static STATIC $<TARGET_OBJECTS:naughty-buffers-objects>)
add_library(naughty-buffers::naughty-buffers-static ALIAS naughty-buffers-static)

set_target_properties(naughty-buffers-static PROPERTIES
    VERSION ${naughty-buffers_VERSION}
    SO_VERSION ${naughty-buffers_VERSION_MAJOR}
    PUBLIC_HEADER "${NAUGHTY_BUFFERS_PUBLIC_HEADERS}"
    OUTPUT_NAME "naughty-buffers-static"
    RUNTIME_OUTPUT_NAME naughty-buffers-${naughty-buffers_VERSION}-static
)

target_include_directories(naughty-buffers-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

if (NOT CMAKE_BUILD_TYPE MATCHES "Release")
  target_compile_options(naughty-buffers-objects PRIVATE
      $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -fvisibility=hidden -Werror>
      $<$<C_COMPILER_ID:MSVC>:/W3 /WX /wd4820 /wd4668 /wd4204>
  )
endif ()

install(TARGETS naughty-buffers naughty-buffers-static
    EXPORT naughty-buffers
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT "Naughty Buffers Runtime"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT "Naughty Buffers Runtime"
    NAMELINK_COMPONENT "Naughty Buffers Development"
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT "Naughty Buffers Development"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/naughty-buffers
    COMPONENT "Naughty Buffers Development"
)

export(
    TARGETS naughty-buffers naughty-buffers-static
    NAMESPACE naughty-buffers::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers-config.cmake"
)

install(EXPORT naughty-buffers
    FILE naughty-buffers-config.cmake
    NAMESPACE naughty-buffers::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/naughty-buffers
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers-config-version.cmake"
    VERSION ${naughty-buffers_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/naughty-buffers
    COMPONENT "Naughty Buffers Development"
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(CTest)
  add_subdirectory(src/tests)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_EXAMPLES)
  add_subdirectory(src/examples)
endif ()

if (BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

include(cmake/naughty-buffers-cpack.cmake)
