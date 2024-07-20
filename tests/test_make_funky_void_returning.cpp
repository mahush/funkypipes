//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_funky_void_returning.hpp"
#include "funkypipes/funky_void.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

// feature callables: move only
TEST(MakeFunkyVoidReturning, nonCopyableCallable_called_works) {
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto decorated_fn = makeFunkyVoidReturning(NonCopyableFn{});

  int result = decorated_fn(1);
  EXPECT_EQ(result, 1);
}

// feature data: value category
TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithLValue_works) {
  auto lambda = [](int) {};

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  FunkyVoid result = decorated_lambda(argument);
}

TEST(MakeFunkyVoidReturning, callableForwardingItsArgument_calledWithLValue_works) {
  auto lambda = [](int arg) { return arg; };

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  const int result = decorated_lambda(argument);
  ASSERT_EQ(result, argument);
}

TEST(MakeFunkyVoidReturning, callableReturningVoid_calledWithRValue_returnsFunkyVoid) {
  auto lambda = [](int) {};
  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  FunkyVoid result = decorated_lambda(std::move(argument));
}

TEST(MakeFunkyVoidReturning, callableForwardingItsArgument_calledWithRValue_returnsArguments) {
  auto lambda = [](int arg) { return arg; };

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  const int result = decorated_lambda(std::move(argument));
  ASSERT_EQ(result, argument);
}

TEST(MakeFunkyVoidReturning, callableForwardingItsArgument_calledWithReference_referenceIsPreserved) {
  auto lambda = [](int& arg) -> int& { return arg; };

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{41};
  int& result = decorated_lambda(argument);
  result++;
  ASSERT_EQ(result, 42);
}

TEST(MakeFunkyVoidReturning, callableForwardingItsArgument_calledWithConstTeference_constReferenceIsPreserved) {
  auto lambda = [](const int& arg) -> const int& { return arg; };

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  int argument{0};
  const int& result = decorated_lambda(argument);
  argument++;
  ASSERT_EQ(result, argument);
}

// feature data: move only
TEST(MakeFunkyVoidReturning, callable_calledWithNonCopyableValue_works) {
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  NonCopyableArg argument{};
  NonCopyableArg result = decorated_lambda(std::move(argument));
}

// feature data: any number of arguments
TEST(MakeFunkyVoidReturning, callableWithMultipleArguments_called_works) {
  auto lambda = [](int, std::string) {};
  auto decorated_lambda = makeFunkyVoidReturning(lambda);

  FunkyVoid result = decorated_lambda(1, "two");
}
