cmake_minimum_required(VERSION 3.16)

project(WaterQualityInspector VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(app

    src/main.cpp
)

add_custom_target(symlink_stylesheets ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/stylesheets" "${CMAKE_BINARY_DIR}/stylesheets")

target_link_libraries(app PRIVATE Qt6::Widgets)

set_target_properties(app PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
