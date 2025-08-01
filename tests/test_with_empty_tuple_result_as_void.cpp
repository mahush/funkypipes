//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <type_traits>

#include "funkypipes/details/with_empty_tuple_result_as_void.hpp"
#include "funkypipes/funky_void.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto withEmptyTupleResultAsVoidFn = [](auto&& arg) { return withEmptyTupleResultAsVoid(std::forward<decltype(arg)>(arg)); };

// feature callables: move only
TEST(WithEmptyTupleResultAsVoid, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(withEmptyTupleResultAsVoidFn));
}

// feature data: value category
TEST(WithEmptyTupleResultAsVoid, callableReturningFunkyVoid_calledWithLValue_returnsVoid) {
  auto lambda = [](int) { return FunkyVoid{}; };

  auto decorated_lambda = withEmptyTupleResultAsVoid(lambda);

  using ResultType = std::invoke_result_t<decltype(decorated_lambda), int&>;
  static_assert(std::is_same_v<ResultType, void>);
}

TEST(WithEmptyTupleResultAsVoid, callableForwardingItsValueArgument_calledWithLValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      withEmptyTupleResultAsVoidFn));
}

TEST(WithEmptyTupleResultAsVoid, callableReturningFunkyVoid_calledWithRValue_returnsVoid) {
  auto lambda = [](int) { return FunkyVoid{}; };
  auto decorated_lambda = withEmptyTupleResultAsVoid(lambda);

  using ResultType = std::invoke_result_t<decltype(decorated_lambda), int&&>;
  static_assert(std::is_same_v<ResultType, void>);
}

TEST(WithEmptyTupleResultAsVoid, callableForwardingItsValueArgument_calledWithRValue_returnsArguments) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      withEmptyTupleResultAsVoidFn));
}

TEST(WithEmptyTupleResultAsVoid, callableForwardingLValueReference_called_lvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingLValueReference_called_lvalueReferenceIsPreserved(
      withEmptyTupleResultAsVoidFn));
}

TEST(WithEmptyTupleResultAsVoid, callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(
          withEmptyTupleResultAsVoidFn));
}

TEST(WithEmptyTupleResultAsVoid, callableForwardingRValueReference_called_rvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingRValueReference_called_rvalueReferenceIsPreserved(
      withEmptyTupleResultAsVoidFn));
}

// feature data: move only
TEST(WithEmptyTupleResultAsVoid, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(withEmptyTupleResultAsVoidFn));
}

