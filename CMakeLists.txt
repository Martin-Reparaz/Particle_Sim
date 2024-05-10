cmake_minimum_required(VERSION 3.10)

project(ParticleSim)

# Specify C++ version
set(CMAKE_CXX_STANDARD 14)

# Search for SDL2 library 
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Source files list
set(SOURCES src/main.cpp src/utils.cpp)

# Generate executable
add_executable(particles ${SOURCES})

# Link SDL2 library
target_link_libraries(particles ${SDL2_LIBRARIES})
