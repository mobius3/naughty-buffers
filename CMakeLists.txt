cmake_minimum_required(VERSION 3.26)
project(naughty-buffers LANGUAGES C VERSION 1.1.0)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(NAUGHTY_BUFFERS_PUBLIC_HEADERS
    include/naughty-buffers/buffer.h
    include/naughty-buffers/array-generator.h
    ${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers/naughty-buffers-export.h
)

add_library(naughty-buffers SHARED
    src/naughty-buffers/buffer.c
    src/naughty-buffers/memory.h
    src/naughty-buffers/memory.c
    ${NAUGHTY_BUFFERS_PUBLIC_HEADERS}
)

generate_export_header(naughty-buffers
    BASE_NAME naughty-buffers
    EXPORT_FILE_NAME naughty-buffers/naughty-buffers-export.h
)

set_target_properties(naughty-buffers PROPERTIES
    C_STANDARD 99
    POSITION_INDEPENDENT_CODE ON
    C_EXTENSIONS OFF
    CXX_EXTENSIONS OFF
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
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

if (NOT CMAKE_BUILD_TYPE MATCHES "Release")
  target_compile_options(naughty-buffers PRIVATE
      $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -fvisibility=hidden -Werror>
      $<$<C_COMPILER_ID:MSVC>:/W3 /WX /wd4820 /wd4668 /wd4204>
  )
endif ()

install(TARGETS naughty-buffers
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
    TARGETS naughty-buffers
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
