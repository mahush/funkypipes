cmake_minimum_required(VERSION 3.12)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(funkypipes LANGUAGES CXX)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

# Suppress warnings for gtest and gmock
if(TARGET gtest)
  target_compile_options(gtest PRIVATE -w)
endif()
if(TARGET gmock)
  target_compile_options(gmock PRIVATE -w)
endif()

# Setup for testing
enable_testing()
add_executable(test_funkypipes 
                               tests/test_make_funky_void_returning.cpp
                               )
target_link_libraries(test_funkypipes PRIVATE gtest_main)
target_include_directories(test_funkypipes PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
)
target_compile_options(test_funkypipes PRIVATE -Wall -Wextra -Wpedantic)

include(GoogleTest)
gtest_discover_tests(test_funkypipes)
