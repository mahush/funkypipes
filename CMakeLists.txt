cmake_minimum_required(VERSION 3.12)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(funkypipes LANGUAGES CXX)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")

# Add the header-only library as an INTERFACE target
add_library(funkypipes INTERFACE)
target_include_directories(funkypipes INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(funkypipes INTERFACE cxx_std_17)

# Create an alias target for easier linking
add_library(funkypipes::funkypipes ALIAS funkypipes)

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
                               tests/details/tuple/test_resolve_rvalue_references.cpp
                               tests/details/tuple/test_separate_tuple_elements.cpp
                               tests/details/tuple/test_try_flatten_tuple.cpp
                               tests/details/tuple/test_tuple_indices_of.cpp
                               tests/details/tuple/test_tuple_traits.cpp
                               tests/test_and_then.cpp
                               tests/test_bind_front.cpp
                               tests/test_at.cpp
                               tests/test_make_arg_optional.cpp
                               tests/test_make_auto_pipe.cpp
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
)

include(GoogleTest)
gtest_discover_tests(test_funkypipes)
