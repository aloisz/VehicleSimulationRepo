cmake_minimum_required(VERSION 3.16)

project(VehicleSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------

find_package(Bullet REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# ------------------------------------------------------------
# Source discovery
# ------------------------------------------------------------

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(VehicleSim
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# ------------------------------------------------------------
# Visual Studio folder organization
# ------------------------------------------------------------

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FILES
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------

target_include_directories(VehicleSim PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party"

    ${BULLET_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

# ------------------------------------------------------------
# Libraries
# ------------------------------------------------------------

target_link_libraries(VehicleSim PRIVATE
    ${BULLET_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# ------------------------------------------------------------
# Compiler settings
# ------------------------------------------------------------

if(MSVC)
    target_compile_options(VehicleSim PRIVATE /W4)
    target_compile_definitions(VehicleSim PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
else()
    target_compile_options(VehicleSim PRIVATE
        -Wall
        -Wextra
    )
endif()

# ------------------------------------------------------------
# Runtime data
# ------------------------------------------------------------

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config")
    add_custom_command(
        TARGET VehicleSim
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/config"
            "$<TARGET_FILE_DIR:VehicleSim>/config"
    )
endif()