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
add_executable(test_funkypipes examples/readme_examples.cpp
                               tests/test_make_arg_optional.cpp
                               tests/test_make_failable_pipe.cpp
                               tests/test_make_raw_pipe.cpp
                               tests/test_make_signature_checking.cpp
                               tests/test_make_skippable.cpp
                               tests/test_make_tuple_packing.cpp
                               tests/test_make_tuple_unpacking.cpp
                               tests/test_traits.cpp)
target_link_libraries(test_funkypipes PRIVATE gtest_main)
target_include_directories(test_funkypipes PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(test_funkypipes)
