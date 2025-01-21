//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/make_callable.hpp"

// Ensure that an overloaded function can be wrapped in a callable object
TEST(MakeCallable, OverloadedFunctionWrappedInCallable_called_works) {
  // given
  auto callable = MAKE_CALLABLE(std::to_string);

  // when
  const auto result = callable(0);

  // then
  ASSERT_EQ(result, "0");
}

// Ensure that a member function call can be wrapped in a callable object
TEST(MakeCallable, memberFunctionCallWrappedInCallable_called_works) {
  // given
  //
  struct Forwarder {
    [[nodiscard]] int forward(int arg) const { return arg; }
  };
  Forwarder forwarder;

  auto callable = MAKE_CALLABLE(forwarder.forward);

  // when
  const auto result = callable(0);

  // then
  ASSERT_EQ(result, 0);
}

