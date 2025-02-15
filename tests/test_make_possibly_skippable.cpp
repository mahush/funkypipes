//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_possibly_skippable.hpp"
#include "predefined/execution_semantics/make_skippable_tests.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makePossiblySkippableFn = [](auto&&... args) {
  return makePossiblySkippable(std::forward<decltype(args)>(args)...);
};

// ########################################################################
// case 1: integration tests (check if base class signatures are available)
// ########################################################################
TEST(MakePossiblySkippable, callableHavingValueArgument_calledWithLValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithLValueOptional_works(makePossiblySkippableFn));
}

TEST(MakePossiblySkippable, callableHavingValueArgument_calledWithRValueOptional_works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithRValueOptional_works(makePossiblySkippableFn));
}

// #################################################
// case 2: feature tests (check if forwarding works)
// #################################################

// feature: callables - move only
TEST(MakePossiblySkippable, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(makePossiblySkippableFn));
}

// feature: data - value categories
TEST(MakePossiblySkippable, callableHavingValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      makePossiblySkippableFn));
}

TEST(MakePossiblySkippable, callablHavingValueArgumente_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      makePossiblySkippableFn));
}
TEST(MakePossiblySkippable, callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableForwardingConstLValueReference_called_constLValueReferenceIsPreserved(
          makePossiblySkippableFn));
}
TEST(MakePossiblySkippable, callableForwardingLValueReference_called_lvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingLValueReference_called_lvalueReferenceIsPreserved(
      makePossiblySkippableFn));
}
TEST(MakePossiblySkippable, callableForwardingRValueReference_called_rvalueReferenceIsPreserved) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingRValueReference_called_rvalueReferenceIsPreserved(
      makePossiblySkippableFn));
}

// feature: data - move only
TEST(MakePossiblySkippable, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(makePossiblySkippableFn));
}
