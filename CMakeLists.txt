cmake_minimum_required(VERSION 3.10)

project(ParticleSim)

# Especifica la versión de C++
set(CMAKE_CXX_STANDARD 14)

# Encuentra la librería SDL2
find_package(SDL2 REQUIRED)

# Incluye las cabeceras de SDL2
include_directories(${SDL2_INCLUDE_DIRS})

# Lista de archivos fuente
set(SOURCES src/main.cpp src/utils.cpp)

# Genera el ejecutable
add_executable(particles ${SOURCES})

# Enlaza con la librería SDL2
target_link_libraries(particles ${SDL2_LIBRARIES})
