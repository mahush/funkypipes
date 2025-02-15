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

#include "funkypipes/fork.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes::test;
using funkypipes::fork;

auto forkFn = [](auto&&... args) { return fork(std::forward<decltype(args)>(args)...); };

// Ensure that forking into two functions is supported
TEST(Fork, twoFunctionsReturningResults_called_twoElemenenttupleReturned) {
  // given
  auto toString = [](auto arg) { return std::to_string(arg); };
  auto doubleFn = [](auto arg) { return arg * 2; };
  auto incrementAndDoubleFn = fork(toString, doubleFn);

  // when
  auto result = incrementAndDoubleFn(2);

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"2"}, 4));
}

// Ensure that forking into three functions is supported
TEST(Fork, threeFunctionsReturningResults_called_threeElementTupleReturned) {
  // given
  auto incrementFn = [](auto arg) { return arg + 1; };
  auto incrementAndDouble = fork(incrementFn, incrementFn, incrementFn);

  // when
  auto result = incrementAndDouble(0);

  // then
  ASSERT_EQ(result, std::make_tuple(1, 1, 1));
}

// Ensure that a single result is returned directly
TEST(Fork, onlyOneFunctionProvidesResult_called_singleResultReturned) {
  // given
  auto swallowFn = [](auto) {};
  auto doubleFn = [](auto arg) { return arg * 2; };
  auto swallowAndDouble = fork(swallowFn, doubleFn);

  // when
  auto result = swallowAndDouble(2);

  // then
  ASSERT_EQ(result, 4);
}

// Ensure that multiple return values are supported
TEST(Fork, twoFunctionsDoublingTheArgument_called_fourElementTupleReturned) {
  // given
  auto doubleFn = [](auto arg) { return std::make_tuple(arg, arg); };
  auto incrementAndDoubleFn = fork(doubleFn, doubleFn);

  // when
  auto result = incrementAndDoubleFn(1);

  // then
  ASSERT_EQ(result, std::make_tuple(1, 1, 1, 1));
}

// feature: callables - move only

// Ensure that move only callable is supported
TEST(Fork, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(forkFn));
}

// feature: data - value categories

// Ensure that returned lvalue references are preserved for single callable
TEST(Fork, callableForwardingReference_calledWithReference_returnsReference) {
  // given
  auto forwardFn = [](const int& arg) -> int& {
    return const_cast<int&>(arg);  // NOLINT cppcoreguidelines-pro-type-const-cast: utilized to simplify test
  };
  auto decorated_fn = fork(forwardFn);

  // when
  int argument{1};
  decltype(auto) result = decorated_fn(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  EXPECT_EQ(result, 1);
  result++;
  EXPECT_EQ(argument, 2);
}

// Ensure that const lvalue references are preserved for single callable
TEST(Fork, callableForwardingConstReference_calledWithConstReference_returnsConstReference) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(forkFn));
}

// Ensure that returned lvalue references are preserved for multiple callables
TEST(Fork, twoFunctionsForwardingReferences_calledWithLValueReference_lvalueReferencesPreserved) {
  // given
  auto forwardFn = [](const int& arg) -> int& {
    return const_cast<int&>(arg);  // NOLINT cppcoreguidelines-pro-type-const-cast: utilized to simplify test
  };
  auto forwardTwiceFn = fork(forwardFn, forwardFn);

  // when
  int argument = 0;
  decltype(auto) result = forwardTwiceFn(argument);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, int&>>);
  ASSERT_EQ(std::get<0>(result), 0);
  ASSERT_EQ(std::get<1>(result), 0);
  std::get<0>(result) = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(argument, 1);
  ASSERT_EQ(std::get<1>(result), 1);
}

// Ensure that const lvalue reference are preserved for multiple callables
TEST(Fork, twoFunctionsForwardingReferences_calledWithConstLValueReference_constLValueReferencesPreserved) {
  // given
  auto forwardFn = [](const auto& arg) -> const auto& { return arg; };
  auto forwardTwiceFn = fork(forwardFn, forwardFn);

  // when
  int argument = 0;
  decltype(auto) result = forwardTwiceFn(std::as_const(argument));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const int&>>);
  ASSERT_EQ(std::get<0>(result), 0);
  ASSERT_EQ(std::get<1>(result), 0);
  argument = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), 1);
}

// Ensure that returning rvalue references is supported for multiple callable
TEST(Fork, twoFunctionsReturningRValueReference_called_returnsRValueReferences) {
  // given
  int valueA{0};
  auto getRValueRefAFn = [&valueA]() mutable -> int&& {
    return std::move(valueA);  // NOLINT hicpp-move-const-arg,performance-move-const-arg
  };

  int valueB{0};
  auto getRValueRefBFn = [&valueB]() mutable -> int&& {
    return std::move(valueB);  // NOLINT hicpp-move-const-arg,performance-move-const-arg
  };

  auto decorated_fn = fork(getRValueRefAFn, getRValueRefBFn);

  // when
  decltype(auto) result = decorated_fn();

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&&, int&&>>);
  ASSERT_EQ(std::get<0>(result), 0);
  ASSERT_EQ(std::get<1>(result), 0);
  valueA = 1;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  valueB = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), 2);
}

// Ensure that rvalue references are not propagated to specified functions
TEST(Fork, twoFunctionsAcceptingRValueReference_called_rvalueReferenceAreResolved) {
  // given and then
  auto verifyArgTypeFn = [](auto&& arg) { static_assert(std::is_same_v<decltype(arg), const int&>); };
  auto verifyArgTypeTwiceFn = fork(verifyArgTypeFn, verifyArgTypeFn);

  // when
  int argument = 0;
  verifyArgTypeTwiceFn(std::move(argument));  // NOLINT hicpp-move-const-arg,performance-move-const-arg
}

// feature: data - any number of arguments

// Ensure that zero arguments are supported for single callable
TEST(Fork, callableReturningValue_calledWithoutArguments_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableReturningValue_calledWithoutArguments_works(forkFn));
}

// Ensure that multiple arguments are supported for single callable
TEST(Fork, callableReturningValue_calledWithMultipleArguments_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableReturningValue_calledWithMultipleArguments_works(forkFn));
}

// Ensure that multiple arguments are supported for multiple callables
TEST(Fork, twoFunctionstakingrMmultipleArguments_called_twoElemenenttupleReturned) {
  // given
  auto toString = [](auto arg1, auto arg2) { return std::to_string(arg1) + std::to_string(arg2); };
  auto incrementAndDoubleFn = fork(toString, toString);

  // when
  auto result = incrementAndDoubleFn(1, 2);

  // then
  ASSERT_EQ(result, std::make_tuple(std::string{"12"}, std::string{"12"}));
}

// feature: data - void return type

// Ensure that void is returned if single callable returns void
TEST(Fork, callableReturningVoid_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableReturningVoid_called_works(forkFn));
}

// Ensure that void is returned if multiple callables return void
TEST(Fork, noFunctionProvidesResult_called_voidReturned) {
  // given
  auto swallowFn = [](auto) {};
  auto swallowAllFn = fork(swallowFn, swallowFn);

  // when
  swallowAllFn(2);
  using ResultType = std::invoke_result_t<decltype(swallowAllFn), int>;

  // then
  static_assert(std::is_same_v<ResultType, void>);
}

