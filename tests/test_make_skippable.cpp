//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_skippable.hpp"
#include "predefined/execution_semantics/make_skippable_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makeSkippableFn = [](auto&&... args) { return makeSkippable(std::forward<decltype(args)>(args)...); };

// feature: callables - generic callables
TEST(MakeSkippable, genericFunctorCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::genericFunctorCallable_calledWithValue_isExecuted(makeSkippableFn));
}

TEST(MakeSkippable, genericLambdaCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::genericLambdaCallable_calledWithValue_isExecuted(makeSkippableFn));
}

TEST(MakeSkippable, overloadedFunctorCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::overloadedFunctorCallable_calledWithValue_isExecuted(makeSkippableFn));
}

// feature: callables - move only
TEST(MakeSkippable, nonCopyableCallable_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::nonCopyableCallable_calledWithValue_isExecuted(makeSkippableFn));
}

// feature: data - value categories
TEST(MakeSkippable, callableHavingValueArgument_calledWithLValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithLValueOptional_works(makeSkippableFn));
}

TEST(MakeSkippable, callableHavingValueArgument_calledWithRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithRValueOptional_works(makeSkippableFn));
}

TEST(MakeSkippable, callableHavingReferenceArgument_calledWithRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingReferenceArgument_calledWithRValueOptional_works(makeSkippableFn));
}

TEST(MakeSkippable, callableProvidingConstReference_called_returnsOptionalConstReference) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableProvidingConstReference_called_returnsOptionalConstReference(makeSkippableFn));
}

TEST(MakeSkippable, callableProvidingReference_called_returnsOptionalReference) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableProvidingReference_called_returnsOptionalReference(makeSkippableFn));
}

// feature: data - move only
TEST(MakeSkippable, callableHavingValueArgument_calledWithNonCopyableRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithNonCopyableRValueOptional_works(makeSkippableFn));
}

// feature: std::optional support
TEST(MakeSkippable, callableReturningOptional_calledWithValue_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningOptional_calledWithValue_isExecuted(makeSkippableFn));
}

// feature: chain breaking
TEST(MakeSkippable, callableReturningValue_calledWithNullopt_isSkipped) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningValue_calledWithNullopt_isSkipped(makeSkippableFn));
}

TEST(MakeSkippable, callableReturningOptional_calledWithNullopt_isSkipped) {
  ASSERT_NO_FATAL_FAILURE(execution_semantics::callableReturningOptional_calledWithNullopt_isSkipped(makeSkippableFn));
}

// TEST(MakeSkippable,
// callableCalledWithUnsupportedArgument_triggersStaticAssert) {
//   auto lambda = [](int value) { return value; };
//
//   auto skippable_lambda = makeSkippable(lambda);
//
//   struct NotAnInt {};
//   auto argument = std::make_optional({NotAnInt});
//   skippable_lambda(argument);
// }
