cmake_minimum_required(VERSION 3.21)

project(ciel-system-daemon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core DBus Network)

qt_standard_project_setup()

qt_add_dbus_adaptor(THEME_DBUS
    org.ciel.Theme.xml
    ThemeService.hpp
    ThemeService
    ThemeAdaptor
    ThemeAdaptor
)

qt_add_dbus_adaptor(FDO_NOTIF_DBUS
    org.freedesktop.Notifications.xml
    NotificationService.hpp
    NotificationService
    NotificationsAdaptor
    NotificationsAdaptor
)

qt_add_dbus_adaptor(CIEL_NOTIF_DBUS
    org.ciel.Notifications.xml
    NotificationService.hpp
    NotificationService
    CielNotificationsAdaptor
    CielNotificationsAdaptor
)

qt_add_dbus_adaptor(STATS_DBUS
    org.ciel.SystemStats.xml
    SystemStatsService.hpp
    SystemStatsService
    SystemStatsAdaptor
    SystemStatsAdaptor
)

qt_add_dbus_adaptor(DOWNLOADS_DBUS
    org.ciel.Downloads.xml
    DownloadService.hpp
    DownloadService
    DownloadsAdaptor
    DownloadsAdaptor
)

add_executable(ciel-system-daemon
    src/main.cpp
    src/Daemon.cpp
    src/ThemeService.cpp
    src/NotificationService.cpp
    src/SystemStatsService.cpp
    src/DownloadService.cpp
    include/Daemon.hpp
    include/ThemeService.hpp
    include/NotificationService.hpp
    include/SystemStatsService.hpp
    include/SystemStatsTypes.hpp
    include/DownloadService.hpp
    ${THEME_DBUS}
    ${FDO_NOTIF_DBUS}
    ${CIEL_NOTIF_DBUS}
    ${STATS_DBUS}
    ${DOWNLOADS_DBUS}
)

target_include_directories(ciel-system-daemon PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ciel-system-daemon PRIVATE
    Qt6::Core
    Qt6::DBus
    Qt6::Network
)
