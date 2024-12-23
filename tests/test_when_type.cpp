//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>

#include "funkypipes/when_type.hpp"

using namespace funkypipes;

TEST(WhenType, tbd0) {
  auto print_int = whenType<int>([](int x) { return x + 1; });
  auto result1 = print_int(42);
  ASSERT_EQ(result1, 43);

  auto result2 = print_int(std::string("Hello"));  // Argument type does not match, returns "Hello"
  ASSERT_EQ(result2, "Hello");
}

