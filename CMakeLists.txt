cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_executable(helloworld
    main.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets)

set_target_properties(helloworld PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

message(STATUS, "WINDEPLOYQT_EXECUTABLE=${WINDEPLOYQT_EXECUTABLE}")

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

if(WIN32)
    message("https://cmake.org/cmake/help/latest/variable/WIN32.html?highlight=win32")
    add_custom_command(
        TARGET helloworld
        POST_BUILD
        COMMAND Qt6::windeployqt
        ARGS $<TARGET_FILE:helloworld>
    )
elseif(APPLE)
    message("https://cmake.org/cmake/help/latest/variable/APPLE.html?highlight=apple")
    add_custom_command(
        TARGET helloworld
        POST_BUILD
        COMMAND Qt6::macdeployqt
        ARGS $<TARGET_FILE:helloworld>
    )
elseif(UNIX)
    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
endif()

get_target_property(_qt6_widgets_location Qt6::Widgets IMPORTED_LOCATION)
execute_process(
    COMMAND "${_qt6_widgets_location}" -query QT_INSTALL_PREFIX
    RESULT_VARIABLE return_code
    OUTPUT_VARIABLE qt6_install_prefix
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS, "WINDEPLOYQT_EXECUTABLE=${_qt6_widgets_location}")
message(STATUS, "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
message(STATUS, "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

# Install
install(TARGETS helloworld)

if(X_VCPKG_APPLOCAL_DEPS_INSTALL)
    install(TARGETS helloworld RUNTIME DESTINATION .)
else()
    install(TARGETS helloworld
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DIRECTORIES ${QT_BIN_DIR}
        RUNTIME DESTINATION .)
endif()

qt_generate_deploy_app_script(
    TARGET helloworld
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

message(STATUS, "WINDEPLOYQT_EXECUTABLE=${deploy_script}")
