cmake_minimum_required(VERSION 3.10)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(funkypipes LANGUAGES CXX)

# Set the C++ standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest gtest_main PROPERTIES
  COMPILE_OPTIONS "-w"
)

# Setup for testing
enable_testing()
add_executable(test_funkypipes tests/test_make_failable_pipe.cpp
                               tests/test_make_raw_pipe.cpp
                               tests/test_make_signature_checking.cpp
                               tests/test_make_skippable.cpp
                               tests/test_make_tuple_unpacking.cpp
                               tests/test_traits.cpp)
target_link_libraries(test_funkypipes PRIVATE gtest_main)
target_include_directories(test_funkypipes PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(test_funkypipes)
