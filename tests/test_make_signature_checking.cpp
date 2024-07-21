//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_signature_checking.hpp"
#include "predefined/signature_propagation/standard_tests.hpp"

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makeSignatureCheckingFn = [](auto&&... args) {
  return makeSignatureChecking(std::forward<decltype(args)>(args)...);
};

// feature: callables - move only
TEST(MakeSignatureChecking, nonCopyableCallable_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::nonCopyableCallable_called_works(makeSignatureCheckingFn));
}

// feature: data - value categories
TEST(MakeSignatureChecking, callable_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableHavingValueArgument_calledWithLValue_works(makeSignatureCheckingFn));
}

TEST(MakeSignatureChecking, callable_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(
      signature_propagation::callableHavingValueArgument_calledWithRValue_works(makeSignatureCheckingFn));
}

// feature: data - move only
TEST(MakeSignatureChecking, callable_calledWithNonCopyableValue_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithNonCopyableValue_works(makeSignatureCheckingFn));
}

// feature: data - any number of arguments
TEST(MakeSignatureChecking, callable_calledWithoutParameter_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithoutParameter_works(makeSignatureCheckingFn));
}

TEST(MakeSignatureChecking, callable_calledWithMultipleParameter_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callable_calledWithMultipleParameter_works(makeSignatureCheckingFn));
}

// feature: data - void return type
TEST(MakeSignatureChecking, callableReturningVoid_called_works) {
  ASSERT_NO_FATAL_FAILURE(signature_propagation::callableReturningVoid_called_works(makeSignatureCheckingFn));
}

// TEST(MakeSignatureChecking, callable_calledWithUnsupportedArgument_triggersStaticAssert) {
//   auto lambda = [](int value) { return value; };
//
//   auto signature_checking_lambda = makeSignatureChecking(lambda);
//
//   struct NotAnInt {};
//   NotAnInt argument{};
//   signature_checking_lambda(argument);
// }
