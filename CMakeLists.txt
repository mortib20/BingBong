cmake_minimum_required(VERSION 3.22)

project(BingBong)

set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR})
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(BingBong ./src/main.cpp ./src/BingBong.h)
target_link_libraries(BingBong ${SDL2_LIBRARIES})