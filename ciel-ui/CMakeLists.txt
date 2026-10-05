cmake_minimum_required(VERSION 3.21)

project(ciel-ui VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Qml DBus ShaderTools)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_standard_project_setup()

file(GLOB_RECURSE UI_QML
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "qml/*.qml"
)

qt_add_library(ciel-ui SHARED)

qt_add_dbus_interface(ciel-ui_THEME_DBUS
    org.ciel.Theme.xml
    ThemeInterface
)

qt_add_dbus_interface(ciel-ui_NOTIF_DBUS
    org.ciel.Notifications.xml
    CielNotificationsInterface
)

target_include_directories(ciel-ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_sources(ciel-ui PRIVATE
    include/AppPaths.hpp
    include/MotionTokens.hpp
    include/ThemeMetrics.hpp
    include/Theme.hpp
    include/NotificationsModel.hpp
    include/cielgraphview.hpp
    src/AppPaths.cpp
    src/Theme.cpp
    src/NotificationsModel.cpp
    src/cielgraphview.cpp
    ${ciel-ui_THEME_DBUS}
    ${ciel-ui_NOTIF_DBUS}
)

target_link_libraries(ciel-ui PUBLIC
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::DBus
)

qt_add_shaders(ciel-ui "ciel_shaders"
    PREFIX
        "/ciel/ui/shaders"
    FILES
        shaders/squircle.frag
)

qt_add_qml_module(ciel-ui
    URI "Ciel.Ui"
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Ciel/Ui"
    SOURCES
        include/AppPaths.hpp
        include/MotionTokens.hpp
        include/ThemeMetrics.hpp
        include/Theme.hpp
        include/NotificationsModel.hpp
        include/cielgraphview.hpp
        src/AppPaths.cpp
        src/Theme.cpp
        src/NotificationsModel.cpp
        src/cielgraphview.cpp
    QML_FILES
        ${UI_QML}
)

enable_testing()
add_subdirectory(test)

