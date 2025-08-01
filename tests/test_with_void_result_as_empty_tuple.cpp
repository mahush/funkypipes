//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "funkypipes/details/with_void_result_as_empty_tuple.hpp"
#include "funkypipes/funky_void.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto withVoidResultAsEmptyTupleFn = [](auto&& arg) {
  return withVoidResultAsEmptyTuple(std::forward<decltype(arg)>(arg));
};

// feature callables: move only
TEST(WithVoidResultAsEmptyTuple, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(withVoidResultAsEmptyTupleFn));
}

// feature data: value category
TEST(WithVoidResultAsEmptyTuple, callableReturningVoid_calledWithLValue_works) {
  auto lambda = [](int) {};

  auto decorated_lambda = withVoidResultAsEmptyTuple(lambda);

  int argument{0};
  decorated_lambda(argument);
}

TEST(WithVoidResultAsEmptyTuple, callableForwardingItsValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableReturningVoid_calledWithRValue_returnsFunkyVoid) {
  auto lambda = [](int) {};
  auto decorated_lambda = withVoidResultAsEmptyTuple(lambda);

  int argument{0};
  auto result = decorated_lambda(std::move(argument));

  static_assert(std::is_same_v<decltype(result), FunkyVoid>);
}

TEST(WithVoidResultAsEmptyTuple, callableForwardingItsValueArgument_calledWithRValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableForwardingLValueReference_called_lvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingLValueReference_called_lvalueReferenceIsPreserved(
      withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(
          withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableForwardingRValueReference_called_rvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingRValueReference_called_rvalueReferenceIsPreserved(
      withVoidResultAsEmptyTupleFn));
}

// feature data: move only
TEST(WithVoidResultAsEmptyTuple, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callable_calledWithNonCopyableValue_works(withVoidResultAsEmptyTupleFn));
}

// feature data: any number of arguments
TEST(WithVoidResultAsEmptyTuple, callableReturningValue_calledWithoutArguments_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithoutArguments_works(withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableReturningValue_calledWithMultipleArguments_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithMultipleArguments_works(withVoidResultAsEmptyTupleFn));
}

TEST(WithVoidResultAsEmptyTuple, callableReturningVoid_calledWithoutArguments_works) {
  auto lambda = []() {};
  auto decorated_lambda = withVoidResultAsEmptyTuple(lambda);

  std::ignore = decorated_lambda();
}

TEST(WithVoidResultAsEmptyTuple, callableReturningVoid_calledWithMultipleArguments_works) {
  auto lambda = [](int, std::string) {};
  auto decorated_lambda = withVoidResultAsEmptyTuple(lambda);

  std::ignore = decorated_lambda(1, "two");
}
