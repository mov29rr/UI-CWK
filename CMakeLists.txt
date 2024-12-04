cmake_minimum_required(VERSION 3.16)

project(WaterQualityInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
qt_standard_project_setup()

qt_add_executable(app

    src/main.cpp
)

include_directories(app PUBLIC "${CMAKE_SOURCE_DIR}/src")

add_custom_target(symlink_stylesheets ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/stylesheets" "${CMAKE_BINARY_DIR}/stylesheets")

target_link_libraries(app PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(app PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
