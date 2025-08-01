//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/with_signature_check.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto withSignatureCheckFn = [](auto&&... args) { return withSignatureCheck(std::forward<decltype(args)>(args)...); };

// feature: callables - move only
TEST(WithSignatureCheck, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(withSignatureCheckFn));
}

// feature: data - value categories
TEST(WithSignatureCheck, callable_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithLValue_returnsArguments(
      withSignatureCheckFn));
}

TEST(WithSignatureCheck, callable_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableForwardingItsValueArgument_calledWithRValue_returnsArguments(
      withSignatureCheckFn));
}

// feature: data - move only
TEST(WithSignatureCheck, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(withSignatureCheckFn));
}

// feature: data - any number of arguments
TEST(WithSignatureCheck, callable_calledWithoutParameter_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithoutArguments_works(withSignatureCheckFn));
}

TEST(WithSignatureCheck, callable_calledWithMultipleParameter_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableReturningValue_calledWithMultipleArguments_works(withSignatureCheckFn));
}

// feature: data - void return type
TEST(WithSignatureCheck, callableReturningVoid_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableReturningVoid_called_works(withSignatureCheckFn));
}

// TEST(WithSignatureCheck, callable_calledWithUnsupportedArgument_triggersStaticAssert) {
//   auto lambda = [](int value) { return value; };
//
//   auto signature_checking_lambda = withSignatureCheck(lambda);
//
//   struct NotAnInt {};
//   NotAnInt argument{};
//   signature_checking_lambda(argument);
// }
