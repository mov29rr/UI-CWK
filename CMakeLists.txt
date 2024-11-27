cmake_minimum_required(VERSION 3.16)

project(WaterQualityInspector VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE DATABASE "src/database/**")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Sql)
qt_standard_project_setup()

qt_add_executable ( app
    src/main.cpp
)

target_link_libraries(app PRIVATE Qt6::Widgets)

set_target_properties(app PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)

# migration script
qt_add_executable (migration
    src/migration/migration.cpp
    ${DATABASE}
)

target_link_libraries(migration PRIVATE Qt6::Widgets Qt6::Core Qt6::Sql)

set_target_properties(migration PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
