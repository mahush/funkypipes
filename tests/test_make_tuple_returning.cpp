//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <tuple>>
#include <type_traits>

#include "funkypipes/details/make_tuple_returning.hpp"

using funkypipes::details::makeTupleReturning;

// Ensure that void returning callables are supported
TEST(MakeTupleReturning, callableReturningVoid_called_returnsEmptyTuple) {
  // given
  auto lambda = [](int) {};
  auto decorated_lambda = makeTupleReturning(lambda);

  // when
  int argument{0};
  auto result = decorated_lambda(std::move(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<>>);
}

// Ensure that value returning callables are supported
TEST(MakeTupleReturning, callableReturningValue_called_returnsTuple) {
  // given
  auto lambda = []() { return 1; };
  auto decorated_lambda = makeTupleReturning(lambda);

  // when
  const std::tuple<int> result = decorated_lambda();

  // then
  EXPECT_EQ(std::get<0>(result), 1);
}

// Ensure that value returning callables are supported
TEST(MakeTupleReturning, callableReturningTuple_called_returnsTuple) {
  // given
  auto lambda = []() { return std::make_tuple(1, std::string{"two"}); };
  auto decorated_lambda = makeTupleReturning(lambda);

  // when
  const std::tuple<int, std::string> result = decorated_lambda();

  // then
  EXPECT_EQ(std::get<0>(result), 1);
  EXPECT_EQ(std::get<1>(result), std::string{"two"});
}

// feature callables: move only
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

// feature data: value category
TEST(MakeTupleReturning, callableForwardingItsValueArgument_calledWithLValue_returnsArgument) {
  // given
  auto lambda = [](int value) { return value; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  int argument{1};
  const std::tuple<int> result = tupleReturningFn(argument);

  // then
  EXPECT_EQ(std::get<0>(result), 1);
}

TEST(MakeTupleReturning, callableForwardingItsValueArgument_calledWithRValue_returnsArgument) {
  // given
  auto lambda = [](int value) { return value; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  int argument{1};
  const std::tuple<int> result = tupleReturningFn(std::move(argument));

  // then
  EXPECT_EQ(std::get<0>(result), 1);
}

// TEST(MakeTupleReturning, callableForwardingItsArgument_calledWithReference_referenceIsPreserved) {
//   auto lambda = [](int& arg) -> int& { return arg; };
//
//   auto tupleReturningFn = makeTupleReturning(lambda);
//
//   int argument{1};
//   std::tuple<std::reference_wrapper<int>> result = tupleReturningFn(argument);
//
//   EXPECT_EQ(std::get<0>(result), 1);
//
//   std::get<0>(result)++;
//   EXPECT_EQ(argument, 2);
// }

// TEST(MakeTupleReturning, callableForwardingItsArgument_calledWithConstReference_constReferenceIsPreserved) {
//   auto lambda = [](const int& arg) -> const int& { return arg; };
//
//   auto tupleReturningFn = makeTupleReturning(lambda);
//
//   int argument{1};
//   std::tuple<std::reference_wrapper<int>> = tupleReturningFn(argument);
//
//   EXPECT_EQ(std::get<0>(result), 1);
//
//   argument++;
//   EXPECT_EQ(std::get<0>(result), 2);
// }

// feature data: move only
TEST(MakeTupleReturning, callable_calledWithNonCopyableValue_NonCopyValueReturned) {
  // given
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;

    int value_;
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
TEST(MakeTupleReturning, callableReturningValue_calledWithoutArguments_works) {
  // given
  auto lambda = []() { return "result"; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  std::tuple<std::string> result = tupleReturningFn();

  // then
  EXPECT_EQ(std::get<0>(result), "result");
}

TEST(MakeTupleReturning, callableReturningValue_calledWithMultipleArguments_works) {
  // given
  auto lambda = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto tupleReturningFn = makeTupleReturning(lambda);

  // when
  std::tuple<int> result = tupleReturningFn(1, 2);

  // then
  EXPECT_EQ(std::get<0>(result), 3);
}
