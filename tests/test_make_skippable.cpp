//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_skippable.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

TEST(MakeSkippable, callable_having_value_argument__called_with_lvalue_optional__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

TEST(MakeSkippable, callable_having_value_argument__called_with_rvalue_optional__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

TEST(MakeSkippable, callable_having_value_argument__called_with_non_copyable_rvalue_optional__works) {
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto skippable_lambda = makeSkippable(lambda);

  auto argument = std::make_optional(NonCopyableArg{});
  std::optional<NonCopyableArg> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
}

TEST(MakeSkippable, callable_having_reference_argument__called_with_rvalue_optional__works) {
  auto lambda = [](const int& value) { return std::to_string(value); };

  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(std::move(argument));
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

TEST(MakeSkippable, callable_returning_value__called_with_nullopt__is_skipped) {
  auto lambda = [](int value) {
    throw std::exception{};
    return std::to_string(value);
  };
  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{std::nullopt};
  std::optional<std::string> res;
  ASSERT_NO_THROW(res = skippable_lambda(argument));
  EXPECT_FALSE(res.has_value());
}

TEST(MakeSkippable, callable_returning_optional__called_with_nullopt__is_skipped) {
  auto lambda = [](int value) {
    throw std::exception{};
    return std::make_optional(std::to_string(value));
  };
  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{std::nullopt};
  std::optional<std::string> res;
  ASSERT_NO_THROW(res = skippable_lambda(argument));
  EXPECT_FALSE(res.has_value());
}

TEST(MakeSkippable, callable_returning_optional__called_with_value__is_executed) {
  auto lambda = [](int value) { return std::make_optional(std::to_string(value)); };

  auto skippable_lambda = makeSkippable(lambda);

  std::optional<int> argument{1};
  std::optional<std::string> res = skippable_lambda(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "1");
}

struct GenericFn {
  template <typename TArg, typename = std::enable_if_t<not IsOptional<TArg>::value>>
  auto operator()(TArg arg) {
    return arg;
  }
};

TEST(MakeSkippable, generic_functor_callable__called_with_value__is_executed) {
  auto skippable_generic_functor = makeSkippable(GenericFn{});

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

TEST(MakeSkippable, generic_lambda_callable__called_with_value__is_executed) {
  auto lambda = [](auto arg) { return arg; };

  auto skippable_lambda = makeSkippable(lambda);

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

TEST(MakeSkippable, overloaded_functor_callable__called_with_value__is_executed) {
  struct OverloadedFn {
    int operator()(int input) const { return input; }
    std::string operator()(std::string input) const { return input; }
  };

  auto skippable_overloaded_fn = makeSkippable(OverloadedFn{});

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

TEST(MakeSkippable, non_copyable_callable__called_with_value__is_executed) {
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto skippable_fn = makeSkippable(NonCopyableFn{});

  std::optional<int> argument{1};
  std::optional<int> res = skippable_fn(argument);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), 1);
}

// TEST(MakeSkippable,
// callable_called_with_unsupported_argument__triggers_static_assert) {
//   auto lambda = [](int value) { return value; };
//
//   auto skippable_lambda = makeSkippable(lambda);
//
//   struct NotAnInt {};
//   auto argument = std::make_optional({NotAnInt});
//   skippable_lambda(argument);
// }

// TEST(MakeSkippable, callable_returning_void__triggers_static_assert) {
//   auto lambda = [](int) {};
//
//   auto skippable_fn = makeSkippable(lambda);
//
//   std::optional<int> argument{1};
//   skippable_fn(argument);
// }

