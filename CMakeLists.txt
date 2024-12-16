cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(box2d REQUIRED)
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE box2d::box2d sfml-graphics sfml-window sfml-system)
