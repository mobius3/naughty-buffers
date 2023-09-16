include(GNUInstallDirs)
include(GenerateExportHeader)

set(I ../../include)
set(NAUGHTY_BUFFERS_PUBLIC_HEADERS
    ${I}/naughty-buffers/buffer.h
    ${I}/naughty-buffers/array-generator.h
    ${I}/naughty-buffers/int-array.h
    ${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers/naughty-buffers-export.h
)

add_library(naughty-buffers SHARED
    buffer.c
    int-array.c
    memory.h
    memory.c
    ${NAUGHTY_BUFFERS_PUBLIC_HEADERS}
)

generate_export_header(naughty-buffers BASE_NAME naughty-buffers EXPORT_FILE_NAME naughty-buffers/naughty-buffers-export.h)

set_target_properties(naughty-buffers PROPERTIES
    C_STANDARD 99
    VERSION ${naughty-buffers_VERSION}
    SO_VERSION ${naughty-buffers_VERSION_MAJOR}
    PUBLIC_HEADER "${NAUGHTY_BUFFERS_PUBLIC_HEADERS}"
    OUTPUT_NAME "naughty-buffers"
)

target_include_directories(naughty-buffers PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
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
    EXPORT naughty-buffers-targets
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