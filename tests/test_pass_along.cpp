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

#include "funkypipes/pass_along.hpp"
#include "utils/move_only_forwarding_fn.hpp"
#include "utils/move_only_struct.hpp"

using funkypipes::passAlong;

// Ensure that passing along one out of one works
TEST(PassAlong, transformingFnDecoratedByPassingAlongAt0_calledWithSingleArgument_transformedAndOriginalReturned) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto passAlong0AndTransform = passAlong<0>(transformingFn);

  // when
  decltype(auto) result = passAlong0AndTransform(0);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(1, 0));
}

// Ensure that passing along one out of many works
TEST(PassAlong, transformingFnDecoratedByPassingAlongAt1_calledWithTwoArguments_transformedAndOriginalAt1Returned) {
  // given
  auto transformingFn = [](int arg1, const std::string& arg2) { return std::to_string(arg1) + arg2; };
  auto passAlong1AndTransform = passAlong<1>(transformingFn);

  // when
  const auto result = passAlong1AndTransform(1, std::string{"two"});

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"1two"}, std::string{"two"}));
}

// Ensure that passing along one out of many by type works
TEST(PassAlong,
     transformingFnDecoratedByPassingAlongAtInt_calledWithStringAndIntArguments_transformedAndOriginalReturned) {
  // given
  auto transformingFn = [](const std::string& arg1, int arg2) { return arg1 + std::to_string(arg2); };
  auto passAlongIntAndTransform = passAlong<int>(transformingFn);

  // when
  const auto result = passAlongIntAndTransform(std::string{"one"}, 2);

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"one2"}, 2));
}

// Ensure that a single result is returned flattened
TEST(PassAlong, consumingFnDecoratedByPassingAlong_calledWithSingleArgument_argumentReturned) {
  // given
  auto consumingFn = [](int) {};
  auto passAlongAndConsume = passAlong<0>(consumingFn);

  // when
  const auto result = passAlongAndConsume(0);

  // then
  ASSERT_EQ(result, 0);
}

// feature: callables - move only

// Ensure that a move only callable is supported
TEST(PassAlong, nonCopyableCallableDecorated_works) {
  // when
  auto decoratedFn = passAlong<0>(MoveOnlyForwardingFn{});

  // then
  const auto result = decoratedFn(0);
  ASSERT_EQ(result, std::make_tuple(0, 0));
}

// Ensure that tuples are returned by value
TEST(PassAlong, consumingFnDecoratedWithPassingAlong_calledWithDouble_tupleWithIntAndDoubleReturnedByValue) {
  // given
  auto consumeFn = [](int, int) {};
  auto passAlongAndConsume = passAlong<0, 1>(consumeFn);

  // when
  decltype(auto) result = passAlongAndConsume(1, 2);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(1, 2));
}

// feature: data - value categories

// Ensure that const lvalue references are preserved when returned flattend
TEST(PassAlong, consumingFnDecoratedByPassingAlong_called_constLValueReferenceIsPreserved) {
  // given
  auto consumingFn = [](const int&) {};
  auto passAlongAndConsume = passAlong<0>(consumingFn);

  // when
  int argument = 0;
  decltype(auto) result = passAlongAndConsume(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  ASSERT_EQ(result, 0);
  argument = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, 1);
}

// Ensure that lvalue references are preserved when returned flattend
TEST(PassAlong, consumingFnDecoratedByPassingAlong_called_lvalueReferenceIsPreserved) {
  // given
  auto referenceIncrementingFn = [](int& arg) { arg++; };
  auto passAlongAndConsume = passAlong<0>(referenceIncrementingFn);

  // when
  int argument = 0;
  decltype(auto) result = passAlongAndConsume(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  ASSERT_EQ(result, 1);
  result++;
  ASSERT_EQ(argument, 2);
}

// Ensure that rvalue references are resolved when returned flattend
TEST(PassAlong, consumingFnDecoratedByPassingAlong_calledWithRValue_RValueReferenceIsResolved) {
  // given
  auto consumingFn = [](const int&) {};
  auto passAlongAndConsume = passAlong<0>(consumingFn);

  // when
  decltype(auto) result = passAlongAndConsume(0);

  // then
  static_assert(std::is_same_v<decltype(result), int>);
  ASSERT_EQ(result, 0);
}

// Ensure that const lvalue references are preserved when returned via tuple
TEST(PassAlong, forwardingFnDecoratedByPassingAlong_called_constLValueReferenceIsPreserved) {
  // given
  auto forwardingFn = [](const int& arg) -> const int& { return arg; };
  auto passAlongAndForward = passAlong<0>(forwardingFn);

  // when
  int argument = 0;
  decltype(auto) result = passAlongAndForward(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const int&>>);
  ASSERT_EQ(result, std::make_tuple(0, 0));
  argument = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, std::make_tuple(1, 1));
}

// Ensure that lvalue references are preserved when returned via tuple
TEST(PassAlong, forwardingFnDecoratedByPassingAlong_called_lvalueReferenceIsPreserved) {
  // given
  auto incrementAndForwardingFn = [](int& arg) -> int& {
    arg++;
    return arg;
  };
  auto passAlongAndIncrementAndForward = passAlong<0>(incrementAndForwardingFn);

  // when
  int argument = 0;
  decltype(auto) result = passAlongAndIncrementAndForward(argument);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, int&>>);
  ASSERT_EQ(result, std::make_tuple(1, 1));
  std::get<0>(result)++;
  std::get<1>(result)++;
  ASSERT_EQ(result, std::make_tuple(3, 3));
}

// Ensure that rvalue references are resolved when returned via tuple
TEST(PassAlong, forwardingFnDecoratedByPassingAlong_calledWithRValue_RValueReferenceIsResolved) {
  // given
  auto forwardingFn = [](int arg) -> int { return arg; };
  auto passAlongAndConsume = passAlong<0>(forwardingFn);

  // when
  decltype(auto) result = passAlongAndConsume(0);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, int>>);
  ASSERT_EQ(result, std::make_tuple(0, 0));
}

// feature: data - move only

// Ensure that a move only argument can be passed along when selected by index
TEST(PassAlong, fnDecoratedToPassAlongByIndex_calledWithMoveOnlyType_works) {
  // given
  auto consumingFn = [](const MoveOnlyStruct&) {};
  auto passAlongAndConsume = passAlong<0>(consumingFn);

  // when
  decltype(auto) result = passAlongAndConsume(MoveOnlyStruct{1});

  // then
  static_assert(std::is_same_v<decltype(result), MoveOnlyStruct>);
  ASSERT_EQ(result.value_, 1);
}

// Ensure that a move only argument can be passed along when selected by type
TEST(PassAlong, fnDecoratedToPassAlongByType_calledWithMoveOnlyType_works) {
  // given
  auto consumingFn = [](const MoveOnlyStruct&) {};
  auto passAlongAndConsume = passAlong<MoveOnlyStruct>(consumingFn);

  // when
  decltype(auto) result = passAlongAndConsume(MoveOnlyStruct{2});

  // then
  static_assert(std::is_same_v<decltype(result), MoveOnlyStruct>);
  ASSERT_EQ(result.value_, 2);
}

