cmake_minimum_required(VERSION 3.10)

project(Newton's_Fall VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# box2d
include_directories("./externals/Box2D/include")
link_directories("./externals/Box2D/build/src")

# sfml
find_package(SFML 2.6.2 COMPONENTS graphics window system REQUIRED)

add_executable(test src/test.cpp)

target_link_libraries(test box2d sfml-graphics sfml-window sfml-system)
