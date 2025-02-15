//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <type_traits>

#include "funkypipes/details/make_funky_void_returning.hpp"
#include "funkypipes/funky_void.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makeFunkyVoidReturningFn = [](auto&& arg) { return makeFunkyVoidReturning(std::forward<decltype(arg)>(arg)); };

// feature callables: move only
TEST(MakeFunkyVoidReturning, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(makeFunkyVoidReturningFn));
}

// feature data: value category
TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithLValue_works) {
  auto lambda = [](int) {};

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  decorated_lambda(argument);
}

TEST(MakeFunkyVoidReturning, callableForwardingItsValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      makeFunkyVoidReturningFn));
}

TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithRValue_returnsFunkyVoid) {
  auto lambda = [](int) {};
  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  auto result = decorated_lambda(std::move(argument));

  static_assert(std::is_same_v<decltype(result), FunkyVoid>);
}

TEST(MakeFunkyVoidReturning, callableForwardingItsValueArgument_calledWithRValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      makeFunkyVoidReturningFn));
}

TEST(MakeFunkyVoidReturning, callableForwardingLValueReference_called_lvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingLValueReference_called_lvalueReferenceIsPreserved(
      makeFunkyVoidReturningFn));
}

TEST(MakeFunkyVoidReturning, callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(
          makeFunkyVoidReturningFn));
}

// feature data: move only
TEST(MakeFunkyVoidReturning, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(makeFunkyVoidReturningFn));
}

// feature data: any number of arguments
TEST(MakeFunkyVoidReturning, callableReturningValue_calledWithoutArguments_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithoutArguments_works(makeFunkyVoidReturningFn));
}

TEST(MakeFunkyVoidReturning, callableReturningValue_calledWithMultipleArguments_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithMultipleArguments_works(makeFunkyVoidReturningFn));
}

TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithoutArguments_works) {
  auto lambda = []() {};
  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  FunkyVoid result = decorated_lambda();
  std::ignore = result;
}

TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithMultipleArguments_works) {
  auto lambda = [](int, std::string) {};
  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  FunkyVoid result = decorated_lambda(1, "two");
  std::ignore = result;
}
