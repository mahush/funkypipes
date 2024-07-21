//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <optional>

#include "funkypipes/funky_void.hpp"
#include "funkypipes/make_pipe.hpp"
#include "predefined/execution_semantics/make_skippable_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::test;

auto andThenFn = [](auto&&... args) { return andThen(std::forward<decltype(args)>(args)...); };

// feature: callables - generic callables
TEST(AndThen, genericFunctorCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::genericFunctorCallable_calledWithValue_isExecuted(andThenFn));
}

TEST(AndThen, genericLambdaCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::genericLambdaCallable_calledWithValue_isExecuted(andThenFn));
}

TEST(AndThen, overloadedFunctorCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::overloadedFunctorCallable_calledWithValue_isExecuted(andThenFn));
}

// feature: callables - move only
TEST(AndThen, nonCopyableCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::nonCopyableCallable_calledWithValue_isExecuted(andThenFn));
}

// feature: data - value categories
TEST(AndThen, callableHavingValueArgument_calledWithLValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableHavingValueArgument_calledWithLValueOptional_works(andThenFn));
}

TEST(AndThen, callableHavingValueArgument_calledWithRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableHavingValueArgument_calledWithRValueOptional_works(andThenFn));
}

TEST(AndThen, callableHavingReferenceArgument_calledWithRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingReferenceArgument_calledWithRValueOptional_works(andThenFn));
}

TEST(AndThen, callableProvidingConstReference_called_returnsOptionalConstReference) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableProvidingConstReference_called_returnsOptionalConstReference(andThenFn));
}

TEST(AndThen, callableProvidingReference_called_returnsOptionalReference) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableProvidingReference_called_returnsOptionalReference(andThenFn));
}

// feature: data - move only
TEST(AndThen, callableHavingValueArgument_calledWithNonCopyableRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithNonCopyableRValueOptional_works(andThenFn));
}

// feature: data - any number of arguments
TEST(MakeSkippable, callable_accecpting_int__called_with_optional_tuple_of_int__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto and_then_lambda = andThen(lambda);

  std::optional<std::tuple<int>> argument{1};
  auto result = and_then_lambda(std::move(argument));
  EXPECT_EQ(result, "1");
}

TEST(MakeSkippable, callable_accecpting_int_and_string__called_with_optional_tuple_of_int_and_string__works) {
  auto lambda = [](int value, std::string string) { return std::to_string(value) + string; };

  auto and_then_lambda = andThen(lambda);

  auto argument = std::make_optional(std::make_tuple<int, std::string>(1, "2"));
  auto result = and_then_lambda(std::move(argument));
  EXPECT_EQ(result, "12");
}

TEST(MakeSkippable, callable_without_parameter__called_with_optional_empty_tuple__works) {
  auto lambda = []() { return 1; };

  auto and_then_lambda = andThen(lambda);

  std::optional<FunkyVoid> empty_tuple = FunkyVoid{};
  auto result = and_then_lambda(std::move(empty_tuple));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value(), 1);
}

// feature: data - void return type
TEST(MakeSkippable, callableReturningVoid_calledWithOptional_returnsFunkyVoid) {
  auto lambda = [](int) -> void {};

  auto and_then_lambda = andThen(lambda);

  std::optional<int> argument{0};
  std::optional<FunkyVoid> result = and_then_lambda(argument);

  ASSERT_TRUE(result.has_value());
}

// feature: std::optional support
TEST(AndThen, callableReturningOptional_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningOptional_calledWithValue_isExecuted(andThenFn));
}

// feature: chain breaking
TEST(AndThen, callableReturningValue_calledWithNullopt_isSkipped) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningValue_calledWithNullopt_isSkipped(andThenFn));
}

TEST(AndThen, callableReturningOptional_calledWithNullopt_isSkipped) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningOptional_calledWithNullopt_isSkipped(andThenFn));
}

