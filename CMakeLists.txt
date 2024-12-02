cmake_minimum_required(VERSION 3.16)

project(WaterQualityInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
file(GLOB_RECURSE MIGRATE_DIR "src/migration/**")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

qt_add_executable (migration
    src/migration/main.cpp
)

target_sources(migration PRIVATE
${DATABASE}
${MIGRATE_DIR}
)

target_link_libraries(migration PRIVATE Qt6::Widgets Qt6::Core Qt6::Sql)

set_target_properties(migration PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
