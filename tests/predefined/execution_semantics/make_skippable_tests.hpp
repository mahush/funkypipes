//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_TESTS_EXECUTION_SEMANTICS_MAKE_SKIPPABLE_TESTS_HPP
#define FUNKYPIPES_TESTS_EXECUTION_SEMANTICS_MAKE_SKIPPABLE_TESTS_HPP

#include <gtest/gtest.h>

#include <optional>

#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/traits.hpp"

namespace funkypipes::test::execution_semantics {

// feature: callables - generic callables
struct GenericFn {
  template <typename TArg>
  auto operator()(TArg arg) {
    return arg;
  }
};

template <typename TFn>
void genericFunctorCallable_calledWithValue_isExecuted(TFn makeSkippableFn) {
  auto skippable_generic_functor = makeSkippableFn(GenericFn{});

  {
    std::optional<int> argument{1};
    std::optional<int> res = skippable_generic_functor(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 1);
  }
  {
    auto argument = std::make_optional("string");
    std::optional<std::string> res = skippable_generic_functor(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "string");
  }
}

template <typename TFn>
void genericLambdaCallable_calledWithValue_isExecuted(TFn makeSkippableFn) {
  auto lambda = [](auto arg) { return arg; };

  auto skippable_lambda = makeSkippableFn(lambda);

  {
    std::optional<int> argument{1};
    std::optional<int> res = skippable_lambda(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 1);
  }
  {
    auto argument = std::make_optional("string");
    std::optional<std::string> res = skippable_lambda(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "string");
  }
}

template <typename TFn>
void overloadedFunctorCallable_calledWithValue_isExecuted(TFn makeSkippableFn) {
  struct OverloadedFn {
    int operator()(int input) const { return input; }
    std::string operator()(std::string input) const { return input; }
  };

  auto skippable_overloaded_fn = makeSkippableFn(OverloadedFn{});

  {
    std::optional<int> argument{1};
    std::optional<int> res = skippable_overloaded_fn(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 1);
  }

  {
    auto argument = std::make_optional("1");
    std::optional<std::string> res = skippable_overloaded_fn(argument);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "1");
  }
}

// feature: callables - move only
template <typename TFn>
void nonCopyableCallable_calledWithValue_isExecuted(TFn makeSkippableFn) {
  struct NonCopyableFn {
    NonCopyableFn() = default;
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto skippable_fn = makeSkippableFn(NonCopyableFn{});

  std::optional<int> argument{1};
  std::optional<int> res = skippable_fn(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), 1);
}

// feature: data - value categories
template <typename TFn>
void callableHavingValueArgument_calledWithLValueOptional_works(TFn makeSkippableFn) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

template <typename TFn>
void callableHavingValueArgument_calledWithRValueOptional_works(TFn makeSkippableFn) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

template <typename TFn>
void callableHavingReferenceArgument_calledWithRValueOptional_works(TFn makeSkippableFn) {
  auto lambda = [](const int& value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

template <typename TFn>
void callableProvidingConstReference_called_returnsOptionalConstReference(TFn makeSkippableFn) {
  auto lambda = [](const int& arg) -> const int& { return arg; };
  auto skippable_lambda = makeSkippableFn(lambda);

  int argument_raw = 1;
  std::optional<std::reference_wrapper<const int>> argument{argument_raw};
  std::optional<std::reference_wrapper<const int>> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), 1);

  const int& result_raw = res.value();
  EXPECT_EQ(result_raw, 1);

  argument_raw++;
  EXPECT_EQ(result_raw, 2);
}

template <typename TFn>
void callableProvidingReference_called_returnsOptionalReference(TFn makeSkippableFn) {
  auto lambda = [](int& arg) -> int& { return arg; };
  auto skippable_lambda = makeSkippableFn(lambda);

  int argument_raw = 1;
  std::optional<std::reference_wrapper<int>> argument{argument_raw};
  std::optional<std::reference_wrapper<int>> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());

  int& result_raw = res.value();
  EXPECT_EQ(result_raw, 1);

  result_raw++;
  EXPECT_EQ(argument_raw, 2);
}

// feature: data - move only
template <typename TFn>
void callableHavingValueArgument_calledWithNonCopyableRValueOptional_works(TFn makeSkippableFn) {
  struct NonCopyableArg {
    NonCopyableArg() = default;
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto skippable_lambda = makeSkippableFn(lambda);

  auto argument = std::make_optional(NonCopyableArg{});
  std::optional<NonCopyableArg> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
}

// feature: std::optional support
template <typename TFn>
void callableReturningOptional_calledWithValue_isExecuted(TFn makeSkippableFn) {
  auto lambda = [](int value) { return std::make_optional(std::to_string(value)); };

  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

// feature: chain breaking
template <typename TFn>
void callableReturningValue_calledWithNullopt_isSkipped(TFn makeSkippableFn) {
  auto lambda = [](int value) {
    throw std::exception{};
    return std::to_string(value);
  };
  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{std::nullopt};
  std::optional<std::string> res;
  ASSERT_NO_THROW(res = skippable_lambda(argument));
  EXPECT_FALSE(res.has_value());
}

template <typename TFn>
void callableReturningOptional_calledWithNullopt_isSkipped(TFn makeSkippableFn) {
  auto lambda = [](int value) {
    throw std::exception{};
    return std::make_optional(std::to_string(value));
  };
  auto skippable_lambda = makeSkippableFn(lambda);

  std::optional<int> argument{std::nullopt};
  std::optional<std::string> res;
  ASSERT_NO_THROW(res = skippable_lambda(argument));
  EXPECT_FALSE(res.has_value());
}

}  // namespace funkypipes::test::execution_semantics

#endif  // FUNKYPIPES_TESTS_EXECUTION_SEMANTICS_MAKE_SKIPPABLE_TESTS_HPP
