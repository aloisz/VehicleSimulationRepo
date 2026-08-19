cmake_minimum_required(VERSION 3.16)
project(VehicleSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

find_package(Bullet REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

set(SOURCES
    src/main.cpp

    src/Core/Application.cpp
    src/Core/Physics.cpp
    src/Core/Input.cpp

    src/Render/Primitives.cpp
    src/Render/DebugDraw.cpp

    src/Objects/Ground.cpp

    src/Vehicle/VehicleCore.cpp
    src/Vehicle/Data/VehicleConfig.cpp
    src/Vehicle/Data/VehicleConfigLoader.cpp
    src/Vehicle/Modules/VehicleInputModule.cpp
    src/Vehicle/Modules/VehicleEngineModule.cpp
    src/Vehicle/Modules/VehicleDrivetrainModule.cpp
    src/Vehicle/Modules/VehicleWheelModule.cpp
    src/Vehicle/Modules/VehiclePhysicsModule.cpp
    src/Vehicle/Modules/VehicleAerodynamicsModule.cpp
)

add_executable(VehicleSim ${SOURCES})

target_include_directories(VehicleSim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${BULLET_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

target_link_libraries(VehicleSim PRIVATE
    ${BULLET_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

if(MSVC)
    target_compile_options(VehicleSim PRIVATE /W4)
    # Silence the deprecation warnings from fixed-function OpenGL.
    target_compile_definitions(VehicleSim PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(VehicleSim PRIVATE -Wall -Wextra)
endif()

# ---------------------------------------------------------------------------
# Runtime data
# ---------------------------------------------------------------------------
