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
add_executable(test_funkypipes examples/readme_examples.cpp
                               tests/details/tuple/test_index_sequence.cpp
                               tests/details/tuple/test_recreate_tuple_from_indices.cpp
                               tests/details/tuple/test_resolve_rvalue_references.cpp
                               tests/details/tuple/test_separate_tuple_elements.cpp
                               tests/details/tuple/test_try_flatten_tuple.cpp
                               tests/details/tuple/test_tuple_indices_of.cpp
                               tests/details/tuple/test_tuple_traits.cpp
                               tests/test_and_then.cpp
                               tests/test_bind_front.cpp
                               tests/test_at.cpp
                               tests/test_pass_along.cpp
                               tests/test_make_arg_optional.cpp
                               tests/test_make_auto_pipe.cpp
                               tests/test_make_callable.cpp
                               tests/test_make_funky_void_removing.cpp
                               tests/test_make_funky_void_returning.cpp
                               tests/test_make_pipe.cpp
                               tests/test_make_possibly_skippable.cpp
                               tests/test_make_raw_pipe.cpp
                               tests/test_make_signature_checking.cpp
                               tests/test_make_skippable.cpp
                               tests/test_make_tuple_packing.cpp
                               tests/test_make_tuple_returning.cpp
                               tests/test_make_tuple_unpacking.cpp
                               tests/test_traits.cpp)
target_link_libraries(test_funkypipes PRIVATE gtest_main)
target_include_directories(test_funkypipes PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
)
target_compile_options(test_funkypipes PRIVATE -Wall -Wextra -Wpedantic)

include(GoogleTest)
gtest_discover_tests(test_funkypipes)
