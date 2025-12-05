cmake_minimum_required(VERSION 3.10)
project(Bare_Metal_Essentials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for headers
include_directories(include)

# Collect source files
file(GLOB SOURCES "src/*.cpp")

# Create library from sources
add_library(os_solutions ${SOURCES})

# Test runner executable
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(test_runner ${TEST_SOURCES})
target_link_libraries(test_runner os_solutions)

enable_testing()
