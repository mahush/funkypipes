//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/with_non_single_args_tupled.hpp"

using funkypipes::details::withNonSingleArgsTupled;

// Ensure that a lvalue reference tuple gets forwarded while preserving its reference
TEST(MakeTuplePacking, callableAcceptingTuple_calledWithLValueTuple_referenceIsPreserved) {
  // given
  auto lambda = [](std::tuple<int>& arg) -> std::tuple<int>& { return arg; };

  auto packing_lambda = withNonSingleArgsTupled(lambda);

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
  auto lambda = [](const std::tuple<int>& arg) -> const std::tuple<int>& {
    return arg;  // NOLINT: bugprone-return-const-ref-from-parameter: no temporary involved here
  };

  auto packing_lambda = withNonSingleArgsTupled(lambda);

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
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  std::tuple<int> argument{1};
  decltype(auto) result = packing_lambda(std::move(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int>&&>);
  EXPECT_EQ(std::get<0>(result), 1);
}

// Ensure that a single lvalue reference element gets forwarded as is
TEST(MakeTuplePacking, callableAcceptingLValueInt_calledWithLValueInt_referenceIsPreserved) {
  // given
  auto forwardFn = [](int& arg) -> int& { return arg; };
  auto packing_lambda = withNonSingleArgsTupled(forwardFn);

  // when
  int argument{1};
  decltype(auto) result = packing_lambda(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  EXPECT_EQ(result, 1);
  result = 2;
  EXPECT_EQ(argument, 2);
}

// Ensure that a single const lvalue reference element gets forwarded as is
TEST(MakeTuplePacking, callableAcceptingConstLValueInt_calledWithConstLValueInt_referenceIsPreserved) {
  // given
  auto forwardFn = [](const int& arg) -> const int& {
    return arg;  // NOLINT bugprone-return-const-ref-from-parameter: is intendet here
  };
  auto packing_lambda = withNonSingleArgsTupled(forwardFn);

  // when
  int argument{1};
  decltype(auto) result = packing_lambda(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  EXPECT_EQ(result, 1);
  argument = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: false positive
  EXPECT_EQ(result, 2);
}

// Ensure that a single rvalue reference element gets forwarded as is
TEST(MakeTuplePacking, callableAcceptingRValueInt_calledWithRValueInt_referenceIsPreserved) {
  // given
  auto forwardFn = [](int&& value) -> int&& { return std::move(value); };
  auto packing_lambda = withNonSingleArgsTupled(forwardFn);

  // when
  int argument{0};  // NOLINT: misc-const-correctness: is about to be moved
  decltype(auto) result =
      packing_lambda(std::move(argument));  // NOLINT hicpp-move-const-arg,performance-move-const-arg

  // then
  static_assert(std::is_same_v<decltype(result), int&&>);
  EXPECT_EQ(result, 0);
  result++;
  EXPECT_EQ(argument,  // NOLINT bugprone-use-after-move,hicpp-invalid-access-moved: actually no move has happend here
            1);
}

// Ensure that multiple lvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfLValueIntInt_calledWithLValueIntInt_referencesArePreserved) {
  // given
  auto lambda = [](std::tuple<int&, int&> arg) { return arg; };
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  int argument0{0};
  int argument1{1};
  decltype(auto) result = packing_lambda(argument0, argument1);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, int&>>);
  EXPECT_EQ(result, std::make_tuple(0, 1));
  std::get<0>(result)++;
  std::get<1>(result)++;
  EXPECT_EQ(argument0, 1);
  EXPECT_EQ(argument1, 2);
}

// Ensure that multiple const lvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfConstLValueIntInt_calledWithConstLValueIntInt_referencesArePreserved) {
  // given
  auto lambda = [](std::tuple<const int&, const int&> arg) { return arg; };
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  int argument0{0};
  int argument1{1};
  decltype(auto) result = packing_lambda(std::as_const(argument0), std::as_const(argument1));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const int&>>);
  EXPECT_EQ(result, std::make_tuple(0, 1));
  argument0++;
  argument1++;
  EXPECT_EQ(result, std::make_tuple(1, 2));
}

// Ensure that multiple rvalue reference element gets forwarded as tuple while preserving element references
TEST(MakeTuplePacking, callableAcceptingTupleOfRValueIntInt_calledWithRValueIntInt_referencesArePreserved) {
  // given
  auto lambda = [](std::tuple<int&&, int&&> arg) { return arg; };
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  int argument0{0};                                    // NOLINT misc-const-correctness: is about to be moved
  int argument1{1};                                    // NOLINT misc-const-correctness: is about to be moved
  auto result = packing_lambda(std::move(argument0),   // NOLINT hicpp-move-const-arg,performance-move-const-arg
                               std::move(argument1));  // NOLINT hicpp-move-const-arg,performance-move-const-arg

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&&, int&&>>);
  EXPECT_EQ(result, std::make_tuple(0, 1));

  std::get<0>(result)++;
  std::get<1>(result)++;
  EXPECT_EQ(argument0,  // NOLINT bugprone-use-after-move,hicpp-invalid-access-moved: actually no move has happend here
            1);
  EXPECT_EQ(argument1,  // NOLINT bugprone-use-after-move,hicpp-invalid-access-moved: actually no move has happend here
            2);
}

// Ensure that returning by value is also supported
TEST(MakeTuplePacking, callableReturningByValue_called_returnedByValue) {
  // given
  auto lambda = [](int arg) -> int { return arg; };
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  const int argument{1};
  decltype(auto) result = packing_lambda(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int>);
  EXPECT_EQ(result, 1);
}

// Ensure that packing a tuple out of zero arguments works
TEST(MakeTuplePacking, callableAccecptingEmptyTuple_calledWithoutArgs_works) {
  // given
  auto lambda = [](std::tuple<>) { return "output"; };
  auto packing_lambda = withNonSingleArgsTupled(lambda);

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
  auto packing_lambda = withNonSingleArgsTupled(lambda);

  // when
  const std::string result = packing_lambda("1", 2);

  // then
  EXPECT_EQ(result, "12");
}

