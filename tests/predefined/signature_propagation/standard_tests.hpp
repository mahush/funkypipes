//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_TESTS_PREDEFINED_SIGNATURE_PROPAGATION_STANDARD_TESTS_HPP
#define FUNKYPIPES_TESTS_PREDEFINED_SIGNATURE_PROPAGATION_STANDARD_TESTS_HPP

#include <gtest/gtest.h>

#include <string>
#include <type_traits>

namespace funkypipes::test::signature_propagation {

// feature: callables - move only
template <typename TFn>
void nonCopyableCallable_called_works(TFn decorating_fn) {
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto decorated_fn = decorating_fn(NonCopyableFn{});

  int argument{1};
  int result = decorated_fn(argument);
  EXPECT_EQ(result, 1);
}

// feature: data - value categories
template <typename TFn>
void callableForwardingItsValueArgument_calledWithLValue_returnsArguments(TFn decorating_fn) {
  auto lambda = [](int value) { return value; };

  auto decorated_fn = decorating_fn(lambda);

  int argument{1};
  const int result = decorated_fn(argument);
  EXPECT_EQ(result, 1);
}

template <typename TFn>
void callableForwardingItsValueArgument_calledWithRValue_returnsArguments(TFn decorating_fn) {
  auto lambda = [](int value) { return value; };

  auto decorated_fn = decorating_fn(lambda);

  int argument{1};
  const int result = decorated_fn(std::move(argument));
  EXPECT_EQ(result, 1);
}

template <typename TFn>
void callableForwardingConstReference_calledWithConstReference_returnsConstReference(TFn decorating_fn) {
  auto lambda = [](const int& arg) -> const int& { return arg; };

  auto decorated_fn = decorating_fn(lambda);

  int argument{1};
  const int& result = decorated_fn(argument);

  EXPECT_EQ(result, 1);

  argument++;
  EXPECT_EQ(result, 2);
}

template <typename TFn>
void callableForwardingReference_calledWithReference_returnsReference(TFn decorating_fn) {
  auto lambda = [](int& arg) -> int& { return arg; };

  auto decorated_fn = decorating_fn(lambda);

  int argument{1};
  int& result = decorated_fn(argument);

  EXPECT_EQ(result, 1);

  result++;
  EXPECT_EQ(argument, 2);
}

// feature: data - move only
template <typename TFn>
void callable_calledWithNonCopyableValue_works(TFn decorating_fn) {
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto decorated_fn = decorating_fn(lambda);

  NonCopyableArg argument{};
  NonCopyableArg result = decorated_fn(std::move(argument));
  std::ignore = result;
}

// feature: data - any number of arguments
template <typename TFn>
void callable_calledWithoutParameter_works(TFn decorating_fn) {
  auto lambda = []() { return "result"; };

  auto decorated_fn = decorating_fn(lambda);

  std::string result = decorated_fn();
  EXPECT_EQ(result, "result");
}

template <typename TFn>
void callable_calledWithMultipleParameter_works(TFn decorating_fn) {
  auto lambda = [](int arg1, int arg2) -> int { return arg1 + arg2; };

  auto decorated_fn = decorating_fn(lambda);

  int result = decorated_fn(1, 2);
  EXPECT_EQ(result, 3);
}

// feature: data - void return type
template <typename TFn>
void callableReturningVoid_called_works(TFn decorating_fn) {
  auto lambda = []() -> void {};

  auto decorated_fn = decorating_fn(lambda);

  decorated_fn();
  using ResultType = std::invoke_result_t<decltype(decorated_fn)>;
  static_assert(std::is_void_v<ResultType>);
}

}  // namespace funkypipes::test::signature_propagation

#endif  // FUNKYPIPES_TESTS_PREDEFINED_SIGNATURE_PROPAGATION_STANDARD_TESTS_HPP
