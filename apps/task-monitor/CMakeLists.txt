cmake_minimum_required(VERSION 3.21)

project(ciel-task-monitor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Quick Qml DBus)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_standard_project_setup()

set(CIEL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set_source_files_properties(org.ciel.SystemStats.xml PROPERTIES
  INCLUDE "../../system-daemon/include/SystemStatsTypes.hpp"
)

qt_add_dbus_interface(DBUS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/org.ciel.SystemStats.xml
    SystemStatsInterface
)

add_executable(ciel-task-monitor
    src/main.cpp
    ${DBUS_SRCS}
)

target_include_directories(ciel-task-monitor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../system-daemon/include"
)

target_include_directories(ciel-task-monitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process
    ${CMAKE_CURRENT_SOURCE_DIR}/src/startup
    ${CMAKE_CURRENT_SOURCE_DIR}/src/boot
    ${CIEL_ROOT}/ciel-ui/include
    ${CIEL_ROOT}/system-daemon/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_directories(ciel-task-monitor PRIVATE
    ${CIEL_ROOT}/ciel-ui/build
)

target_link_libraries(ciel-task-monitor PRIVATE
    ciel-ui
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::DBus
)

set_target_properties(ciel-task-monitor PROPERTIES
    BUILD_RPATH "${CIEL_ROOT}/ciel-ui/build"
    INSTALL_RPATH "${CIEL_ROOT}/ciel-ui/build"
)

target_compile_definitions(ciel-task-monitor PRIVATE
    CIEL_UI_IMPORT_PATH="${CIEL_ROOT}/ciel-ui/build"
)

qt_add_qml_module(ciel-task-monitor
    URI "Ciel.TaskMonitor"
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Ciel/TaskMonitor"

    SOURCES
        src/process/ProcessTableModel.hpp
        src/process/ProcessTableModel.cpp
        src/startup/StartupAppsModel.hpp
        src/startup/StartupAppsModel.cpp
        src/boot/BootPerformanceModel.hpp
        src/boot/BootPerformanceModel.cpp

    QML_FILES
        qml/Main.qml
        qml/ui/toolbar/Toolbar.qml
        qml/ui/ProcessView/ProcessView.qml
        qml/ui/PerformanceView/PerformanceView.qml
        qml/ui/StartupView/StartupView.qml
        qml/ui/BootTimeView/BootTimeView.qml
        qml/ui/SettingsView/SettingsView.qml
)
