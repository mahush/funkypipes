//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <utility>

#include "funkypipes/details/make_tuple_packing.hpp"

using funkypipes::details::makeTuplePacking;

// Ensure that a lvalue reference tuple gets forwarded while preserving its reference
TEST(MakeTuplePacking, callableAcceptingTuple_calledWithLValueTuple_referenceIsPreserved) {
  // given
  auto lambda = [](std::tuple<int>& arg) -> std::tuple<int>& { return arg; };

  auto packing_lambda = makeTuplePacking(lambda);

  // when
  std::tuple<int> argument{1};
  decltype(auto) result = packing_lambda(argument);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int>&>);
  std::get<0>(result) = 2;
  EXPECT_EQ(std::get<0>(argument), 2);
}

// Ensure that a const lvalue reference tuple gets forwarded while preserving its reference
TEST(MakeTuplePacking, callableAcceptingTuple_calledWithConstLValueTuple_referenceIsPreserved) {
  // given
  auto lambda = [](const std::tuple<int>& arg) -> const std::tuple<int>& { return arg; };

  auto packing_lambda = makeTuplePacking(lambda);

  // when
  std::tuple<int> argument{1};
  decltype(auto) result = packing_lambda(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), const std::tuple<int>&>);
  EXPECT_EQ(std::get<0>(result), 1);
  std::get<0>(argument) = 2;
  EXPECT_EQ(std::get<0>(result), 2);
}

// Ensure that a rvalue reference tuple gets forwarded while preserving its reference
TEST(MakeTuplePacking, callableAcceptingTuple_calledWithRValueTuple_referenceIsPreserved) {
  // given
  auto lambda = [](std::tuple<int>&& arg) -> std::tuple<int>&& { return std::move(arg); };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  std::tuple<int> argument{1};
  decltype(auto) result = packing_lambda(std::move(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int>&&>);
  EXPECT_EQ(std::get<0>(result), 1);
}

// Ensure that a lvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfInt_calledWithLValueInt_referenceIsPreserved) {
  // given
  auto lambda = [](std::tuple<int&> arg) -> int& { return std::get<0>(arg); };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  int argument{1};
  decltype(auto) result = packing_lambda(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  EXPECT_EQ(result, 1);
  result = 2;
  EXPECT_EQ(argument, 2);
}

// Ensure that a const lvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfInt_calledWithConstLValueInt_referenceIsPreserved) {
  // given
  auto lambda = [](std::tuple<const int&> arg) -> const int& { return std::get<0>(arg); };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  int argument{1};
  decltype(auto) result = packing_lambda(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  EXPECT_EQ(result, 1);
  argument = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: false positive
  EXPECT_EQ(result, 2);
}

// Ensure that a rvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfInt_calledWithRValueInt_referenceIsPreserved) {
  //
  // given
  //
  struct NonCopyableOrMovableArg {
    explicit NonCopyableOrMovableArg(int value) : value_{value} {};
    ~NonCopyableOrMovableArg() = default;
    NonCopyableOrMovableArg(const NonCopyableOrMovableArg&) = delete;
    NonCopyableOrMovableArg(NonCopyableOrMovableArg&&) = delete;
    NonCopyableOrMovableArg& operator=(const NonCopyableOrMovableArg&) = delete;
    NonCopyableOrMovableArg& operator=(NonCopyableOrMovableArg&&) = delete;

    int value_;  // NOLINT misc-non-private-member-variables-in-classes: intended
  };

  auto lambda = [](std::tuple<NonCopyableOrMovableArg&&> value) -> NonCopyableOrMovableArg&& {
    return std::get<0>(std::move(value));
  };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  NonCopyableOrMovableArg argument{1};  // NOLINT: misc-const-correctness: is about to be moved
  decltype(auto) result = packing_lambda(std::move(argument));

  // then
  static_assert(std::is_same_v<decltype(result), NonCopyableOrMovableArg&&>);
  EXPECT_EQ(result.value_, 1);
}

// Ensure that returning by value works
TEST(MakeTuplePacking, callableReturningByValue_called_returnedByValue) {
  // given
  auto lambda = [](std::tuple<const int> arg) -> int { return std::get<0>(arg); };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  int argument{1};
  decltype(auto) result = packing_lambda(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), int>);
  EXPECT_EQ(result, 1);
}

// Ensure that packing a tuple out of zero arguments works
TEST(MakeTuplePacking, callableAccecptingEmptyTuple_calledWithoutArgs_works) {
  // given
  auto lambda = [](std::tuple<>) { return "output"; };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  const std::string result = packing_lambda();

  // then
  EXPECT_EQ(result, "output");
}

// Ensure that packing a tuple out of multiple arguments works
TEST(MakeTuplePacking, callableAccecptingTupleOfIntAndString_calledWithIntAndString_works) {
  // given
  auto lambda = [](std::tuple<std::string, int> value) {
    return std::get<std::string>(value) + std::to_string(std::get<int>(value));
  };
  auto packing_lambda = makeTuplePacking(lambda);

  // when
  const std::string result = packing_lambda("1", 2);

  // then
  EXPECT_EQ(result, "12");
}
