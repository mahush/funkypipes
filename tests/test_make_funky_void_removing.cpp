//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <type_traits>

#include "funkypipes/details/make_funky_void_removing.hpp"
#include "funkypipes/funky_void.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makeFunkyVoidRemovingFn = [](auto&& arg) { return makeFunkyVoidRemoving(std::forward<decltype(arg)>(arg)); };

// feature callables: move only
TEST(MakeFunkyVoidRemoving, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(makeFunkyVoidRemovingFn));
}

// feature data: value category
TEST(MakeFunkyVoidRemoving, callableReturningFunkyVoid_calledWithLValue_returnsVoid) {
  auto lambda = [](int) { return FunkyVoid{}; };

  auto decorated_lambda = makeFunkyVoidRemoving(lambda);

  using ResultType = std::invoke_result_t<decltype(decorated_lambda), int&>;
  static_assert(std::is_same_v<ResultType, void>);
}

TEST(MakeFunkyVoidRemoving, callableForwardingItsValueArgument_calledWithLValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      makeFunkyVoidRemovingFn));
}

TEST(MakeFunkyVoidRemoving, callableReturningFunkyVoid_calledWithRValue_returnsVoid) {
  auto lambda = [](int) { return FunkyVoid{}; };
  auto decorated_lambda = makeFunkyVoidRemoving(lambda);

  using ResultType = std::invoke_result_t<decltype(decorated_lambda), int&&>;
  static_assert(std::is_same_v<ResultType, void>);
}

TEST(MakeFunkyVoidRemoving, callableForwardingItsValueArgument_calledWithRValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      makeFunkyVoidRemovingFn));
}

TEST(MakeFunkyVoidRemoving, callableForwardingLValueReference_called_lvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingLValueReference_called_lvalueReferenceIsPreserved(
      makeFunkyVoidRemovingFn));
}

TEST(MakeFunkyVoidRemoving, callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(
          makeFunkyVoidRemovingFn));
}

TEST(MakeFunkyVoidRemoving, callableForwardingRValueReference_called_rvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingRValueReference_called_rvalueReferenceIsPreserved(
      makeFunkyVoidRemovingFn));
}

// feature data: move only
TEST(MakeFunkyVoidRemoving, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(makeFunkyVoidRemovingFn));
}

