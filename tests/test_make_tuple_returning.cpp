//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/make_tuple_returning.hpp"

using funkypipes::details::makeTupleReturning;

// feature: data - void return type
// Ensure that void returning callables are supported
TEST(MakeTupleReturning, callableReturningVoid_called_returnsEmptyTuple) {
  // given
  auto lambda = [](int) {};
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  decltype(auto) result = tupleReturningFn(0);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<>>);
}

// Ensure that value returning callables are supported
TEST(MakeTupleReturning, callableReturningValue_called_returnsTuple) {
  // given
  auto lambda = []() { return 1; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  decltype(auto) result = tupleReturningFn();

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int>>);
  EXPECT_EQ(std::get<0>(result), 1);
}

// feature: data - value categories
// Ensure that lvalue reference returning callables are supported
TEST(MakeTupleReturning, callableReturningLValueReference_called_referenceIsPreserved) {
  // given
  auto lambda = [](int& arg) -> int& { return arg; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  int argument{1};
  decltype(auto) result = tupleReturningFn(argument);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&>>);
  EXPECT_EQ(std::get<0>(result), 1);
  std::get<0>(result) = 2;
  EXPECT_EQ(argument, 2);
}

// feature: data - value categories
// Ensure that const lvalue reference returning callables are supported
TEST(MakeTupleReturning, callableReturningConstLValueReference_called_referenceIsPreserved) {
  // given
  auto lambda = [](const int& arg) -> const int& { return arg; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  int argument{1};
  decltype(auto) result = tupleReturningFn(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&>>);
  EXPECT_EQ(std::get<0>(result), 1);
  argument = 2;
  EXPECT_EQ(std::get<0>(result), 2);
}

// feature: data - value categories
// Ensure that rvalue reference returning callables are supported
TEST(MakeTupleReturning, callableReturningRValueReference_called_referenceIsPreserved) {
  // given
  auto lambda = [](int&& arg) -> int&& { return std::move(arg); };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  int argument{1};  // Note: object to move from must still exist when accessing result
  decltype(auto) result = tupleReturningFn(std::move(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&&>>);
  EXPECT_EQ(std::get<0>(result), 1);
  std::get<0>(result) = 2;
  EXPECT_EQ(argument,
            2);  // NOLINT bugprone-use-after-move,hicpp-invalid-access-moved: actually no move has happend here
}

// Ensure that tuple returning callables are supported
TEST(MakeTupleReturning, callableReturningTuple_called_returnsTuple) {
  // given
  auto lambda = []() { return std::make_tuple(1, std::string{"two"}); };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  decltype(auto) result = tupleReturningFn();

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, std::string>>);
  EXPECT_EQ(std::get<0>(result), 1);
  EXPECT_EQ(std::get<1>(result), std::string{"two"});
}

// feature callables: move only
// Ensure that a move-only callable can be wrapped
TEST(MakeTupleReturning, nonCopyableCallable_called_works) {
  // given
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };
  auto tupleReturningFn = makeTupleReturning(NonCopyableFn{});

  // when
  std::tuple<int> result = tupleReturningFn(1);

  // then
  EXPECT_EQ(std::get<0>(result), 1);
}

// feature data: move only
// Ensure that a non copyable argument can get passed through
TEST(MakeTupleReturning, callable_calledWithNonCopyableValue_NonCopyValueReturned) {
  // given
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;

    int value_;  // NOLINT misc-non-private-member-variables-in-classes: inteded
  };
  auto lambda = [](NonCopyableArg arg) { return arg; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  NonCopyableArg argument{1};
  std::tuple<NonCopyableArg> result = tupleReturningFn(std::move(argument));

  // then
  ASSERT_EQ(std::get<0>(result).value_, 1);
}

// feature data: any number of arguments
// Ensure that zero arguments are supported
TEST(MakeTupleReturning, callableReturningValue_calledWithoutArguments_works) {
  // given
  auto lambda = []() { return "result"; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  std::tuple<std::string> result = tupleReturningFn();

  // then
  EXPECT_EQ(std::get<0>(result), "result");
}

// feature data: any number of arguments
// Ensure that multiple arguments are supported
TEST(MakeTupleReturning, callableReturningValue_calledWithMultipleArguments_works) {
  // given
  auto lambda = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  std::tuple<int> result = tupleReturningFn(1, 2);

  // then
  EXPECT_EQ(std::get<0>(result), 3);
}
