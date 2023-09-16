include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Setup install of exported targets
install(EXPORT naughty-buffers-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/naughty-buffers
)

# Macro to write config
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers-config-version.cmake"
    VERSION ${naughty-buffers_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Setup install of version config
install(
    FILES
    cmake/naughty-buffers-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/naughty-buffers-config-version.cmake"
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/naughty-buffers
    COMPONENT "Naughty Buffers Development"
)