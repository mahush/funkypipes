//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/compose_raw.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

int incrementFn(int value) {
  value++;
  return value;
};

TEST(ComposeRaw, single_callable__composed__works) {
  auto composition = composeRaw(incrementFn);

  auto result = composition(0);
  ASSERT_EQ(result, 1);
}

TEST(ComposeRaw, two_callables__composed__works) {
  auto composition = composeRaw(incrementFn, incrementFn);

  auto result = composition(0);
  ASSERT_EQ(result, 2);
}

TEST(ComposeRaw, three_callables__composed__works) {
  auto composition = composeRaw(incrementFn, incrementFn, incrementFn);

  auto result = composition(0);
  ASSERT_EQ(result, 3);
}

TEST(ComposeRaw, lambdas_composition_with_value_argument__called_with_lvalue__works) {
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto composition = composeRaw(lambda_1, lambda_2);

  int argument{0};
  auto result = composition(argument);
  ASSERT_EQ(result, "0");
}

TEST(ComposeRaw, lambdas_composition_with_value_argument__called_with_rvalue__works) {
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto composition = composeRaw(lambda_1, lambda_2);

  int argument{1};
  auto result = composition(std::move(argument));
  ASSERT_EQ(result, "1");
}

TEST(ComposeRaw, callables_forwarding_const_refererence__composed__const_references_are_preserved) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto composition = composeRaw(lambda, lambda);

  int argument{0};
  const int& result = composition(argument);
  ASSERT_EQ(result, 0);

  argument++;
  ASSERT_EQ(result, 1);
}

TEST(ComposeRaw, callables_forwarding_refererence__composed__references_are_preserved) {
  auto lambda = [](int& value) -> int& { return value; };

  auto composition = composeRaw(lambda, lambda);

  int argument{1};
  int& result = composition(argument);
  ASSERT_EQ(result, 1);

  result++;
  ASSERT_EQ(argument, 2);
}
