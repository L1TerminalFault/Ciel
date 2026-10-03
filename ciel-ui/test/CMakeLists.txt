add_executable(ciel-test main.cpp)

# Tell the linker to look in the parent build directory for libciel-ui.so
set_target_properties(ciel-test PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_BINARY_DIR}/.."
    INSTALL_RPATH "$ORIGIN/.."
)

target_link_libraries(ciel-test PRIVATE
    ciel-ui
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
)

qt_add_qml_module(ciel-test
    URI "Ciel.Test"
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Ciel/Test"
    QML_FILES
        Main.qml
)
