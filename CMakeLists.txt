include(Helpers.cmake)

cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

message(STATUS, "WINDEPLOYQT_EXECUTABLE=${WINDEPLOYQT_EXECUTABLE}")

# Set target `Qt6::windeployqt` if not found
if(Qt6_FOUND AND WIN32 AND TARGET Qt6::qmake AND NOT TARGET Qt6::windeployqt)
    get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt6_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt6_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(Qt6::windeployqt IMPORTED)

        set_target_properties(Qt6::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()

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
    #     TARGET ${PROJECT_NAME}
    #     POST_BUILD
    #     COMMAND Qt6::macdeployqt
    #     ARGS $<TARGET_BUNDLE_DIR:${PROJECT_NAME}> # Full path to the bundle directory (my.app, my.framework, or my.bundle) where tgt is the name of a target.
    # )
elseif(UNIX)
    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
endif()

# Install
if(WIN32)
    # Install the executable into "${CMAKE_INSTALL_PREFIX}/bin".
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

# install(TARGETS ${PROJECT_NAME}
# RUNTIME_DEPENDENCIES
# PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
# POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
# DIRECTORIES ${QT_BIN_DIR}
# RUNTIME DESTINATION .)
elseif(APPLE)
    # Install the executable into "${CMAKE_INSTALL_PREFIX}".
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
    # install(CODE "
    #     include(BundleUtilities)
    #     fixup_bundle(${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app \"\" \"\")
    # " COMPONENT Runtime)

# install(TARGETS ${PROJECT_NAME}
# BUNDLE DESTINATION .
# RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
# )

# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin BUNDLE DESTINATION ${CMAKE_SOURCE_DIR})
elseif(UNIX)
    # Install the executable into "${CMAKE_INSTALL_PREFIX}/bin".
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# Generate the deployment script for the target MyApp.
qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

# Call the deployment script on "cmake --install".
install(SCRIPT ${deploy_script})

message(STATUS, "deploy_script=${deploy_script}")
message(STATUS, "CMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}")
message(STATUS, "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS, "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")

# print_all_variables()