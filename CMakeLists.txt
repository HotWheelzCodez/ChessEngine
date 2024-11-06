cmake_minimum_required(VERSION 3.12)
project(ChessEngine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Define paths for source, headers, and libraries
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

# Include the header directory
include_directories(${INCLUDE_DIR})

# Collect all source files
file(GLOB_RECURSE PROJECT_SOURCES "${SRC_DIR}/*.cpp")

# Add executable for the project
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Platform-specific settings
if(WIN32)
    set(RAYLIB_LIBRARY ${LIB_DIR}/raylib.lib)
    set(WINDOWS_LIBS winmm gdi32 kernel32 opengl32)
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY} ${WINDOWS_LIBS})
else()
    set(RAYLIB_PATH ${LIB_DIR}/libraylib.a)
    find_library(RAYLIB_LIBRARY raylib HINTS ${RAYLIB_PATH})
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
