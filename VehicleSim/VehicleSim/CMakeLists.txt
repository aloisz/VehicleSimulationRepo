cmake_minimum_required(VERSION 3.20)
project(VehicleSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages installed by vcpkg
find_package(Bullet REQUIRED)
find_package(GLM REQUIRED)
find_package(GLUT REQUIRED)

# Add executable
add_executable(VehicleSim
    src/main.cpp
    src/Physics.cpp
)

# Include directories
target_include_directories(VehicleSim PRIVATE
    ${BULLET_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(VehicleSim PRIVATE
    ${BULLET_LIBRARIES}
    ${GLUT_LIBRARIES}
)
