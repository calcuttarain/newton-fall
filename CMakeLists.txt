cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(box2d REQUIRED)
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
find_package(pybind11 REQUIRED)
find_package(nlohmann_json REQUIRED)

# project
include_directories(
    "${CMAKE_SOURCE_DIR}/game/objects"
    "${CMAKE_SOURCE_DIR}/game/graphics"
    "${CMAKE_SOURCE_DIR}/game/core"
    "${CMAKE_SOURCE_DIR}/game/procedural"
)
file(GLOB OBJECTS_SOURCES
    "${CMAKE_SOURCE_DIR}/game/objects/*.cpp"
    "${CMAKE_SOURCE_DIR}/game/graphics/*.cpp"
    "${CMAKE_SOURCE_DIR}/game/core/*.cpp"
    "${CMAKE_SOURCE_DIR}/game/procedural/*.cpp"

)

# Main executable target configuration
add_executable(${PROJECT_NAME} game/main.cpp ${OBJECTS_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    box2d::box2d 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
    nlohmann_json::nlohmann_json
)

# Python bindings configuration
pybind11_add_module(bindmodule
    rl/bindings.cpp
    ${OBJECTS_SOURCES}
)

target_include_directories(bindmodule PRIVATE
    ${CMAKE_SOURCE_DIR}/game/objects
    ${CMAKE_SOURCE_DIR}/game/graphics
    ${CMAKE_SOURCE_DIR}/game/core
    ${CMAKE_SOURCE_DIR}/game/procedural
)

target_link_libraries(bindmodule PRIVATE
    box2d::box2d
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    nlohmann_json::nlohmann_json
)
