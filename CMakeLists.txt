cmake_minimum_required(VERSION 3.10)
project(OS_Interview_Prep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for headers
include_directories(include)

# Collect source files
file(GLOB SOURCES "src/*.cpp")

# Create library from sources
add_library(os_solutions ${SOURCES})

# Test runner executable
add_executable(test_runner tests/test_runner.cpp)
target_link_libraries(test_runner os_solutions)

enable_testing()
