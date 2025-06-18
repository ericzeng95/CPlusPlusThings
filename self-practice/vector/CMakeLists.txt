cmake_minimum_required(VERSION 3.10)
project(MyVectorProject LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Include directories for your project
include_directories(${CMAKE_SOURCE_DIR})

# Source files for the vector library
add_library(myvector STATIC
    vector.cpp
)

# Test executable
add_executable(vector_tests
    vector.t.cpp
)

# Link test executable with myvector library and Google Test
target_link_libraries(vector_tests
    myvector
    GTest::GTest
    GTest::Main
    pthread
)

# Add test to CTest
add_test(NAME VectorTests COMMAND vector_tests)