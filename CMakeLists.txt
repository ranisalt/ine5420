cmake_minimum_required(VERSION 3.5)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(cg CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(-Wall -Wno-parentheses)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})

add_library(cg
    displayfile.cpp
    shapes.cpp
    matrixmanager.cpp
    )

target_link_libraries(cg ${GTKMM_LIBRARIES})

add_library(ui
    includeobjectwindow.cpp
    mainwindow.cpp
    numericentry.cpp
    viewportdraw.cpp
    axiswindow.cpp
    turningpointwindow.cpp
    choosepointdialog.cpp
    )
target_link_libraries(ui cg)

add_executable(main main.cpp)
target_link_libraries(main cg ui ${GTKMM_LIBRARIES})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
