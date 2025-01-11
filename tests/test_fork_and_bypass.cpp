//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/fork_and_bypass.hpp"
#include "utils/move_only_struct.hpp"

using funkypipes::forkAndBypass;

// Ensure that forking any out of one works
TEST(Fork, transformingFnDecoratedByForkingAny_calledWithSingleArgument_transformedAndOriginalReturned) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto forkAndTransform = forkAndBypass(transformingFn);

  // when
  decltype(auto) result = forkAndTransform(0);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(1, 0));
}

// Ensure that forking one out of one works
TEST(Fork, transformingFnDecoratedByForkingAt0_calledWithSingleArgument_transformedAndOriginalReturned) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto fork0AndTransform = forkAndBypass<0>(transformingFn);

  // when
  const auto result = fork0AndTransform(0);

  // then
  ASSERT_EQ(result, std::make_tuple(1, 0));
}

// Ensure that forking one out of many works
TEST(Fork, transformingFnDecoratedByForkingAt1_calledWithTwoArguments_transformedAndOriginalAt1Returned) {
  // given
  auto transformingFn = [](int arg1, const std::string& arg2) { return std::to_string(arg1) + arg2; };
  auto fork1AndTransform = forkAndBypass<1>(transformingFn);

  // when
  const auto result = fork1AndTransform(1, std::string{"two"});

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"1two"}, std::string{"two"}));
}

// Ensure that forking one out of many by type works
TEST(Fork, transformingFnDecoratedByForkingAtInt_calledWithStringAndIntArguments_transformedAndOriginalReturned) {
  // given
  auto transformingFn = [](const std::string& arg1, int arg2) { return arg1 + std::to_string(arg2); };
  auto forkIntAndTransform = forkAndBypass<int>(transformingFn);

  // when
  const auto result = forkIntAndTransform(std::string{"one"}, 2);

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"one2"}, 2));
}

// Ensure that a single result is returned flattened
TEST(Fork, tbda5) {
  // given
  auto consumingFn = [](int) {};
  auto forkAndConsume = forkAndBypass(consumingFn);

  // when
  const auto result = forkAndConsume(0);

  // then
  ASSERT_EQ(result, 0);
}

// feature: callables - move only

// Ensure that a move only callable is supported
TEST(Fork, nonCopyableCallableDecorated_works) {
  // when
  struct NonCopyableFn {
    NonCopyableFn() = default;
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    void operator()(int) const {}
  };
  auto decoratedFn = forkAndBypass(NonCopyableFn{});

  // then
  const auto result = decoratedFn(0);
  ASSERT_EQ(result, 0);
}

// Ensure that tuples are returned by value
TEST(Fork, consumingFnDecoratedWithForkingAll_calledWithDouble_tupleWithIntAndDoubleReturnedByValue) {
  // given
  auto consumeFn = [](int, int) {};
  auto forkAndConsume = forkAndBypass(consumeFn);

  // when
  decltype(auto) result = forkAndConsume(1, 2);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(1, 2));
}

// feature: data - value categories

// Ensure that const lvalue references are preserved when returned flattend
TEST(Fork, consumingFnDecoratedByForkingAny_called_constLValueReferenceIsPreserved) {
  // given
  auto consumingFn = [](const int&) {};
  auto forkAndConsume = forkAndBypass(consumingFn);

  // when
  int argument = 0;
  decltype(auto) result = forkAndConsume(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  ASSERT_EQ(result, 0);
  argument = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, 1);
}

// Ensure that lvalue references are preserved when returned flattend
TEST(Fork, consumingFnDecoratedByForkingAny_called_lvalueReferenceIsPreserved) {
  // given
  auto referenceIncrementingFn = [](int& arg) { arg++; };
  auto forkAndConsume = forkAndBypass(referenceIncrementingFn);

  // when
  int argument = 0;
  decltype(auto) result = forkAndConsume(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  ASSERT_EQ(result, 1);
  result++;
  ASSERT_EQ(argument, 2);
}

// Ensure that rvalue references are resolved when returned flattend
TEST(Fork, consumingFnDecoratedByForkingAny_calledWithRValue_RValueReferenceIsResolved) {
  // given
  auto consumingFn = [](const int&) {};
  auto forkAndConsume = forkAndBypass(consumingFn);

  // when
  decltype(auto) result = forkAndConsume(0);

  // then
  static_assert(std::is_same_v<decltype(result), int>);
  ASSERT_EQ(result, 0);
}

// Ensure that const lvalue references are preserved when returned via tuple
TEST(Fork, forwardingFnDecoratedByForkingAny_called_constLValueReferenceIsPreserved) {
  // given
  auto forwardingFn = [](const int& arg) -> const int& { return arg; };
  auto forkAndForward = forkAndBypass(forwardingFn);

  // when
  int argument = 0;
  decltype(auto) result = forkAndForward(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const int&>>);
  ASSERT_EQ(result, std::make_tuple(0, 0));
  argument = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, std::make_tuple(1, 1));
}

// Ensure that lvalue references are preserved when returned via tuple
TEST(Fork, forwardingFnDecoratedByForkingAny_called_lvalueReferenceIsPreserved) {
  // given
  auto incrementAndForwardingFn = [](int& arg) -> int& {
    arg++;
    return arg;
  };
  auto forkAndIncrementAndForward = forkAndBypass(incrementAndForwardingFn);

  // when
  int argument = 0;
  decltype(auto) result = forkAndIncrementAndForward(argument);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, int&>>);
  ASSERT_EQ(result, std::make_tuple(1, 1));
  std::get<0>(result)++;
  std::get<1>(result)++;
  ASSERT_EQ(result, std::make_tuple(3, 3));
}

// Ensure that rvalue references are resolved when returned via tuple
TEST(Fork, forwardingFnDecoratedByForkingAny_calledWithRValue_RValueReferenceIsResolved) {
  // given
  auto forwardingFn = [](int arg) -> int { return arg; };
  auto forkAndConsume = forkAndBypass(forwardingFn);

  // when
  decltype(auto) result = forkAndConsume(0);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(0, 0));
}

// feature: data - move only

// Ensure that a move only argument can be bypassed when all forked
TEST(Fork, fnDecoratedToForkAny_calledWithMoveOnlyType_works) {
  // given
  auto consumingFn = [](const MoveOnlyStruct&) {};
  auto forkAndConsume = forkAndBypass(consumingFn);

  // when
  decltype(auto) result = forkAndConsume(MoveOnlyStruct{1});

  // then
  static_assert(std::is_same_v<decltype(result), MoveOnlyStruct>);
  ASSERT_EQ(result.value_, 1);
}

// Ensure that a move only argument can be bypassed when selected by index
TEST(Fork, fnDecoratedToForkByIndex_calledWithMoveOnlyType_works) {
  // given
  auto consumingFn = [](const MoveOnlyStruct&) {};
  auto forkAndConsume = forkAndBypass<0>(consumingFn);

  // when
  decltype(auto) result = forkAndConsume(MoveOnlyStruct{2});

  // then
  static_assert(std::is_same_v<decltype(result), MoveOnlyStruct>);
  ASSERT_EQ(result.value_, 2);
}

// Ensure that a move only argument can be bypassed when selected by type
TEST(Fork, fnDecoratedToForkByType_calledWithMoveOnlyType_works) {
  // given
  auto consumingFn = [](const MoveOnlyStruct&) {};
  auto forkAndConsume = forkAndBypass<MoveOnlyStruct>(consumingFn);

  // when
  decltype(auto) result = forkAndConsume(MoveOnlyStruct{3});

  // then
  static_assert(std::is_same_v<decltype(result), MoveOnlyStruct>);
  ASSERT_EQ(result.value_, 3);
}

