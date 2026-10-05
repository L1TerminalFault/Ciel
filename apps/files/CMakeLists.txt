cmake_minimum_required(VERSION 3.21)

project(ciel-files VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Quick Qml DBus)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_standard_project_setup()

set(CIEL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")


file(GLOB_RECURSE FILES_SOURCES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

file(GLOB_RECURSE FILES_QML
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "qml/*.qml"
)

list(REMOVE_ITEM FILES_SOURCES "src/main.cpp")

add_executable(files
    src/main.cpp
)

target_include_directories(files PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CIEL_ROOT}/ciel-ui/include
    ${CIEL_ROOT}/system-daemon/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_directories(files PRIVATE
    ${CIEL_ROOT}/ciel-ui/build
)

target_link_libraries(files PRIVATE
    ciel-ui
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::DBus
)

set_target_properties(files PROPERTIES
    BUILD_RPATH "${CIEL_ROOT}/ciel-ui/build"
    INSTALL_RPATH "${CIEL_ROOT}/ciel-ui/build"
)

target_compile_definitions(files PRIVATE
    CIEL_UI_IMPORT_PATH="${CIEL_ROOT}/ciel-ui/build"
)

qt_add_qml_module(files
    URI "Ciel.Files"
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Ciel/Files"
    SOURCES ${FILES_SOURCES}
    QML_FILES ${FILES_QML}
)

