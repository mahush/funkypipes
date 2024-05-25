//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_raw_pipe.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

int incrementFn(int value) {
  value++;
  return value;
};

TEST(MakeRawPipe, single_callable__composed__works) {
  auto pipe = makeRawPipe(incrementFn);

  auto result = pipe(0);
  ASSERT_EQ(result, 1);
}

TEST(MakeRawPipe, two_callables__composed__works) {
  auto pipe = makeRawPipe(incrementFn, incrementFn);

  auto result = pipe(0);
  ASSERT_EQ(result, 2);
}

TEST(MakeRawPipe, three_callables__composed__works) {
  auto pipe = makeRawPipe(incrementFn, incrementFn, incrementFn);

  auto result = pipe(0);
  ASSERT_EQ(result, 3);
}

TEST(MakeRawPipe, lambdas_composition_with_value_argument__called_with_lvalue__works) {
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makeRawPipe(lambda_1, lambda_2);

  int argument{0};
  auto result = pipe(argument);
  ASSERT_EQ(result, "0");
}

TEST(MakeRawPipe, lambdas_composition_with_value_argument__called_with_rvalue__works) {
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makeRawPipe(lambda_1, lambda_2);

  int argument{1};
  auto result = pipe(std::move(argument));
  ASSERT_EQ(result, "1");
}

TEST(MakeRawPipe, callables_forwarding_const_refererence__composed__const_references_are_preserved) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto pipe = makeRawPipe(lambda, lambda);

  int argument{0};
  const int& result = pipe(argument);
  ASSERT_EQ(result, 0);

  argument++;
  ASSERT_EQ(result, 1);
}

TEST(MakeRawPipe, callables_forwarding_refererence__composed__references_are_preserved) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeRawPipe(lambda, lambda);

  int argument{1};
  int& result = pipe(argument);
  ASSERT_EQ(result, 1);

  result++;
  ASSERT_EQ(argument, 2);
}

TEST(MakeRawPipe, non_copyable_callables__composed__works) {
  struct NonCopyableFn {
    NonCopyableFn() = default;
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto pipe = makeRawPipe(NonCopyableFn{}, NonCopyableFn{}, NonCopyableFn{});
  EXPECT_EQ(0, pipe(0));
}

TEST(MakeRawPipe, callables_with_non_copyable_arguments__composed__works) {
  struct NonCopyableArg {
    explicit NonCopyableArg(int arg) : arg_{arg} {}
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
    int arg_;
  };
  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto pipe = makeRawPipe(lambda, lambda, lambda);
  auto res = pipe(NonCopyableArg{0});

  EXPECT_EQ(0, res.arg_);
}
