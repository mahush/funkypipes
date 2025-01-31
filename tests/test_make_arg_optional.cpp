//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_arg_optional.hpp"
#include "utils/move_only_struct.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

TEST(MakeArgOptional, callable_having_optional_argument__called_with_lvalue_optional__works) {
  auto lambda = [](std::optional<int> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  std::optional<int> argument{1};
  std::optional<int> res = arg_optional(argument);

  EXPECT_EQ(res, 1);
}

TEST(MakeArgOptional, callable_having_optional_argument__called_with_rvalue_optional__works) {
  auto lambda = [](std::optional<int> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  std::optional<int> argument{1};
  std::optional<int> res = arg_optional(std::move(argument));

  EXPECT_EQ(res, 1);
}

TEST(MakeArgOptional, callable_having_optional_argument__called_with_non_copyable_rvalue_optional__works) {
  auto lambda = [](std::optional<MoveOnlyStruct> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  auto argument = std::make_optional(MoveOnlyStruct{0});
  std::optional<MoveOnlyStruct> res = arg_optional(std::move(argument));

  ASSERT_TRUE(res.has_value());
  ASSERT_EQ(res->value_, 0);
}

TEST(MakeArgOptional, callable_having_optional_argument__called_with_lvalue_value__works) {
  auto lambda = [](std::optional<int> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  int argument{1};
  std::optional<int> res = arg_optional(argument);

  EXPECT_EQ(res, 1);
}

TEST(MakeArgOptional, callable_having_optional_argument__called_with_rvalue_value__works) {
  auto lambda = [](std::optional<int> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  int argument{1};
  std::optional<int> res = arg_optional(std::move(argument));

  EXPECT_EQ(res, 1);
}

TEST(MakeArgOptional, callable_having_optional_argument__called_with_non_copyable_rvalue_value__works) {
  auto lambda = [](std::optional<MoveOnlyStruct> arg) { return arg; };
  auto arg_optional = makeArgOptional(lambda);

  auto argument = MoveOnlyStruct{0};
  std::optional<MoveOnlyStruct> res = arg_optional(std::move(argument));

  ASSERT_TRUE(res.has_value());
  ASSERT_EQ(res->value_, 0);
}
