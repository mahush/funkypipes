//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>

#include "funkypipes/details/make_tuple_packing.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

TEST(MakeTuplePacking, callable_accecpting_tuple__called_with_lvalue_tuple__works) {
  auto lambda = [](std::tuple<int> value) { return std::get<int>(value); };

  auto packing_lambda = makeTuplePacking(lambda);

  std::tuple<int> argument{1};
  int res = packing_lambda(argument);
  EXPECT_EQ(res, 1);
}

TEST(MakeTuplePacking, callable_accecpting_tuple__called_with_rvalue_tuple__works) {
  auto lambda = [](std::tuple<int> value) { return std::get<int>(value); };

  auto packing_lambda = makeTuplePacking(lambda);

  std::tuple<int> argument{1};
  int res = packing_lambda(std::move(argument));
  EXPECT_EQ(res, 1);
}

TEST(MakeTuplePacking, callable_accecpting_tuple_of_int__called_with_lvalue_int__works) {
  auto lambda = [](std::tuple<int> value) { return std::get<int>(value); };

  auto packing_lambda = makeTuplePacking(lambda);

  int argument{1};
  int res = packing_lambda(argument);
  EXPECT_EQ(res, 1);
}

TEST(MakeTuplePacking, callable_accecpting_tuple_of_int__called_with_rvalue_int__works) {
  auto lambda = [](std::tuple<int> value) { return std::get<int>(value); };

  auto packing_lambda = makeTuplePacking(lambda);

  int argument{1};
  int res = packing_lambda(std::move(argument));
  EXPECT_EQ(res, 1);
}

TEST(MakeTuplePacking, callable_accecpting_tuple_of_int_and_string__called_with_int_and_string__works) {
  auto lambda = [](std::tuple<std::string, int> value) {
    return std::get<std::string>(value) + std::to_string(std::get<int>(value));
  };

  auto packing_lambda = makeTuplePacking(lambda);

  std::string res = packing_lambda("1", 2);
  EXPECT_EQ(res, "12");
}

TEST(MakeTuplePacking,
     callable_accepting_const_reference_of_tuple__called_with_const_reference__reference_is_preserved) {
  auto lambda = [](const std::tuple<int>& value) -> const std::tuple<int>& { return value; };

  auto packing_lambda = makeTuplePacking(lambda);

  std::tuple<int> argument{1};
  const std::tuple<int>& result = packing_lambda(argument);
  EXPECT_EQ(std::get<int>(result), 1);

  std::get<int>(argument)++;
  ASSERT_EQ(std::get<int>(result), 2);
}

TEST(MakeTuplePacking, callable_accepting_reference_of_tuple__called_with_reference__reference_is_preserved) {
  auto lambda = [](std::tuple<int>& value) -> std::tuple<int>& { return value; };

  auto packing_lambda = makeTuplePacking(lambda);

  std::tuple<int> argument{1};
  std::tuple<int>& result = packing_lambda(argument);
  EXPECT_EQ(std::get<int>(result), 1);

  std::get<int>(result)++;
  ASSERT_EQ(std::get<int>(argument), 2);
}

