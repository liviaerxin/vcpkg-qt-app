include(Helpers.cmake)

cmake_minimum_required(VERSION 3.21.1)

project(
    helloworld
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default value when cmake-variables are not passed as arguments from CLI.
# When using `option(), these variables must not be already set as a normal or cache variable
# Equal to: set(BUILD_SHARED_LIBS ON CACHE BOOL "Build libfoo shared library")
option(BUILD_SHARED_LIBS "Default BUILD_SHARED_LIBS to ON" ON)

if(MSVC)
    # Create dlls on Windows
    option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Default CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to ON" ON)
endif()

if(APPLE)
    option(CMAKE_MACOSX_BUNDLE "Default CMAKE_MACOSX_BUNDLE to ON" ON)
endif()

# Change `CMAKE_INSTALL_PREFIX` default to `${PROJECT_SOURCE_DIR}/install` as this is an application-based standalone project.
# NOTE: `option()` will not work here becuase `CMAKE_INSTALL_PREFIX` is already cached into `CMakeCache.txt` before running `option()`!
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install CACHE PATH "Force path to set CMAKE_INSTALL_PREFIX" FORCE)
endif()


# Find packages go here.
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

# qt_add_executable will do `list(APPEND install_rpath "@executable_path/../Frameworks")` in function `_qt_internal_finalize_macos_app()`
# qt_add_executable(${PROJECT_NAME}
#     main.cpp
# )

# NOTE: When using `add_executable`, it must add `` for running `qt_generate_deploy_app_script()` successfully
add_executable(${PROJECT_NAME}
    main.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    # INSTALL_RPATH "@executable_path/../Frameworks"
    INSTALL_RPATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug>/lib"
    INSTALL_RPATH_USE_LINK_PATH ON
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Post Build to add platform dependencies
if(WIN32)
    message("https://cmake.org/cmake/help/latest/variable/WIN32.html?highlight=win32")
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND Qt6::windeployqt
        ARGS $<TARGET_FILE:${PROJECT_NAME}>
    )
elseif(APPLE)
    message("https://cmake.org/cmake/help/latest/variable/APPLE.html?highlight=apple")
    print_target_properties(Qt6::macdeployqt)

# add_custom_command(
# TARGET ${PROJECT_NAME}
# POST_BUILD
# COMMAND Qt6::macdeployqt
# ARGS $<TARGET_BUNDLE_DIR:${PROJECT_NAME}> # Full path to the bundle directory (my.app, my.framework, or my.bundle) where tgt is the name of a target.
# )
elseif(UNIX)
    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
endif()

# Install
if(WIN32)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION .
    )

# install(TARGETS ${PROJECT_NAME}
# RUNTIME_DEPENDENCIES
# PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
# POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
# DIRECTORIES ${QT_BIN_DIR}
# RUNTIME DESTINATION .)
elseif(APPLE)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    )

elseif(UNIX)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# # Generate the deployment script for the target MyApp.
# qt_generate_deploy_app_script(
#     TARGET ${PROJECT_NAME}
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )

# # Call the deployment script on "cmake --install".
# install(SCRIPT ${deploy_script})


message(STATUS "BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
message(STATUS "CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=${CMAKE_WINDOWS_EXPORT_ALL_SYMBOsLS}")
message(STATUS "CMAKE_MACOSX_BUNDLE=${CMAKE_MACOSX_BUNDLE}")
message(STATUS "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "deploy_script=${deploy_script}")
message(STATUS "CMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}")
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_BUILD_RPATH=${CMAKE_BUILD_RPATH}")
message(STATUS "CMAKE_INSTALL_RPATH=${CMAKE_INSTALL_RPATH}")

message(STATUS "CMAKE_SYSTEM_PREFIX_PATH=${CMAKE_SYSTEM_PREFIX_PATH}")
message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

message(STATUS "CMAKE_SYSTEM_INCLUDE_PATH=${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "CMAKE_INCLUDE_PATH=${CMAKE_INCLUDE_PATH}")

message(STATUS "CMAKE_SYSTEM_LIBRARY_PATH=${CMAKE_SYSTEM_LIBRARY_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH}")

message(STATUS "Qt6_FOUND=${Qt6_FOUND}")
message(STATUS "Qt6_DIR=${Qt6_DIR}")
message(STATUS "Qt6_INCLUDE_DIRS=${Qt6_INCLUDE_DIRS}")
message(STATUS "Qt6_CONFIG=${Qt6_CONFIG}")
message(STATUS "Qt6_VERSION=${Qt6_VERSION}")
message(STATUS "Qt6_VERSION_MAJOR=${Qt6_VERSION_MAJOR}")
message(STATUS "Qt6_VERSION_MINOR=${Qt6_VERSION_MINOR}")

message(STATUS "_VCPKG_INSTALLED_DIR=${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
message(STATUS "VCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET}")
message(STATUS "VCPKG_INSTALLED_DIR=${VCPKG_INSTALLED_DIR}")
message(STATUS "VCPKG_MANIFEST_MODE=${VCPKG_MANIFEST_MODE}")
message(STATUS "VCPKG_MANIFEST_DIR=${VCPKG_MANIFEST_DIR}")
message(STATUS "VCPKG_MANIFEST_INSTALL=${VCPKG_MANIFEST_INSTALL}")
message(STATUS "VCPKG_INSTALLED_DIR=${VCPKG_INSTALLED_DIR}")

# print_all_variables()