cmake_minimum_required(VERSION 3.12)
project(ChessEngine)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

set(RAYLIB_PATH "include/raylib")
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
find_library(RAYLIB_LIBRARY raylib HINTS ${RAYLIB_PATH})
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY})
