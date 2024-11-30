//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>

#include "funkypipes/consume_at.hpp"

using namespace funkypipes;

TEST(ConsumeAt, tbd0) {
  auto consumingFn = [](int) {};
  auto decoratedConsumingFn = consumeAt<0>(consumingFn);

  decoratedConsumingFn(2);

  using ResultType = std::invoke_result_t<decltype(decoratedConsumingFn), int>;
  static_assert(std::is_same_v<ResultType, void>, "");
}

TEST(ConsumeAt, tbd) {
  auto consumingFn = [](int) {};
  auto decoratedConsumingFn = consumeAt<1>(consumingFn);

  const auto result = decoratedConsumingFn("one", 2);

  ASSERT_EQ(result, "one");
}

TEST(ConsumeAt, tbd2) {
  bool consumed{false};
  auto consumingFn = [&consumed](int) { consumed = true; };

  auto decoratedConsumingFn = consumeAt<1>(consumingFn);

  auto result = decoratedConsumingFn('1', 2, "three");

  ASSERT_EQ(result, std::make_tuple('1', "three"));
}

