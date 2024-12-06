cmake_minimum_required(VERSION 3.16)

project(WaterQualityInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE DATABASE "src/database/**")

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core Sql)
qt_standard_project_setup()

set(SRC_FILES

    src/main.cpp
    
    src/core/application.cpp
    src/core/window.cpp

    src/pages/page.cpp
    src/pages/persistentOrganicPollutants.cpp
    src/pages/pollutantOverview.cpp

    src/charts/complianceColourCodedLineGraph.cpp
)

qt_add_executable(app ${SRC_FILES})

include_directories(app PUBLIC "${CMAKE_SOURCE_DIR}/src")

add_custom_target(symlink_stylesheets ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/stylesheets" "${CMAKE_BINARY_DIR}/stylesheets")

target_link_libraries(app PRIVATE Qt6::Widgets Qt6::Charts Qt6::Sql)

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
