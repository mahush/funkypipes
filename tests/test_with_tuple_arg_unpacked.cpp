//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/with_tuple_arg_unpacked.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

TEST(WithTupleArgUnpacked, callable_accecpting_int__called_with_tuple_of_int__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  std::tuple<int> argument{1};
  std::string res = unpacking_lambda(std::move(argument));
  EXPECT_EQ(res, "1");
}

TEST(WithTupleArgUnpacked, callable_accecpting_int_and_string__called_with_tuple_of_int_and_string__works) {
  auto lambda = [](int value, std::string string) { return std::to_string(value) + string; };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  auto argument = std::make_tuple<int, std::string>(1, "2");
  std::string res = unpacking_lambda(std::move(argument));
  EXPECT_EQ(res, "12");
}

TEST(WithTupleArgUnpacked, callable_accepting_int__called_with_int__works) {
  auto lambda = [](int value) { return value; };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  int argument{0};
  int res = unpacking_lambda(std::move(argument));
  EXPECT_EQ(res, 0);
}

TEST(WithTupleArgUnpacked, callable_without_parameter__called_with_empty_tuple__works) {
  auto lambda = []() { return true; };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  std::tuple<> empty_tuple;
  std::optional<bool> res = unpacking_lambda(std::move(empty_tuple));
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), true);
}

TEST(WithTupleArgUnpacked, callable_accepting_const_reference__called_with_const_reference__works) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  int argument{1};
  const int& result = unpacking_lambda(argument);
  EXPECT_EQ(result, 1);

  argument++;
  ASSERT_EQ(result, 2);
}

TEST(WithTupleArgUnpacked, callable_accepting_reference__called_with_reference__works) {
  auto lambda = [](int& value) -> int& { return value; };

  auto unpacking_lambda = withTupleArgUnpacked(lambda);

  int argument{1};
  int& result = unpacking_lambda(argument);
  EXPECT_EQ(result, 1);

  result++;
  ASSERT_EQ(argument, 2);
}
