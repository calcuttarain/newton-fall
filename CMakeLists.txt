cmake_minimum_required(VERSION 3.10)

project(Newton's_Fall VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
link_directories(lib)

add_executable(box2d_test src/box2d_test.cpp)

target_link_libraries(box2d_test box2d)
