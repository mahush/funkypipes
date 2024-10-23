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

using namespace funkypipes;
using namespace funkypipes::details;
using namespace funkypipes::test;

auto makePossiblySkippableFn = [](auto&&... args) {
  return makePossiblySkippable(std::forward<decltype(args)>(args)...);
};

// ########################################################################
// case 1: integration tests (check if base class signatures are available)
// ########################################################################
TEST(MakePossiblySkippable, callable_having_value_argument__called_with_lvalue_optional__works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithLValueOptional_works(makePossiblySkippableFn));
}

TEST(MakePossiblySkippable, callable_having_value_argument__called_with_rvalue_optional__works) {
  ASSERT_NO_FATAL_FAILURE(
      execution_semantics::callableHavingValueArgument_calledWithRValueOptional_works(makePossiblySkippableFn));
}

// #####################
// case 2: feature tests
// #####################

// feature: callables - move only
TEST(MakePossiblySkippable, non_copyable_callable__called_with_value__is_executed) {
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto skippable_fn = makePossiblySkippable(NonCopyableFn{});

  int argument{1};
  int result = skippable_fn(argument);
  EXPECT_EQ(result, 1);
}

// feature: data - value categories
TEST(MakePossiblySkippable, callable_having_value_argument__called_with_lvalue__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makePossiblySkippable(lambda);

  int argument{1};
  std::string result = skippable_lambda(argument);
  EXPECT_EQ(result, "1");
}
TEST(MakePossiblySkippable, callable_having_value_argument__called_with_rvalue__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makePossiblySkippable(lambda);

  int argument{1};
  std::string result = skippable_lambda(std::move(argument));
  EXPECT_EQ(result, "1");
}
TEST(MakePossiblySkippable, callable_forwarding_const_reference__called_with_const_reference__returns_const_reference) {
  auto lambda = [](const int& arg) -> const int& { return arg; };

  auto skippable_lambda = makePossiblySkippable(lambda);

  int argument{1};
  const int& result = skippable_lambda(argument);

  EXPECT_EQ(result, 1);

  argument++;
  EXPECT_EQ(result, 2);
}
TEST(MakePossiblySkippable, callable_forwarding_reference__called_with_reference__returns_reference) {
  auto lambda = [](int& arg) -> int& { return arg; };

  auto skippable_lambda = makePossiblySkippable(lambda);

  int argument{1};
  int& result = skippable_lambda(argument);

  EXPECT_EQ(result, 1);

  result++;
  EXPECT_EQ(argument, 2);
}

// feature: data - move only
TEST(MakePossiblySkippable, callable_having_value_argument__called_with_non_copyable_rvalue_value__works) {
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto skippable_lambda = makePossiblySkippable(lambda);

  auto argument = NonCopyableArg{};
  NonCopyableArg result = skippable_lambda(std::move(argument));
  (void)result;
}
