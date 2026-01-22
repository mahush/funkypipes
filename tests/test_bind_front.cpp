//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>

#include "funkypipes/bind_front.hpp"

using namespace funkypipes;

TEST(BindFront, TwoArgumentLambdaWithFirstArgumentBound_calledWithOneArgument_works) {
  auto lambda = [](int arg1, std::string arg2) { return std::to_string(arg1) + arg2; };
  auto boundLambda = bindFront(lambda, 1);

  const auto result = boundLambda("2");

  ASSERT_EQ(result, "12");
}

TEST(BindFront, TwoArgumentLambdaWithAllArgumentBound_calledWithoutArguments_works) {
  auto lambda = [](int arg1, std::string arg2) { return std::to_string(arg1) + arg2; };
  auto boundLambda = bindFront(lambda, 3, "4");

  const auto result = boundLambda();

  ASSERT_EQ(result, "34");
}

TEST(BindFront, TwoArgumentLambdaWithoutArgumentsBound_calledWithTwoArguments_works) {
  auto lambda = [](int arg1, std::string arg2) { return std::to_string(arg1) + arg2; };
  auto boundLambda = bindFront(lambda);

  const auto result = boundLambda(5, "6");
  ASSERT_EQ(result, "56");
}

struct Foo {
  int multiply(int x) const { return x * factor; }
  int factor;
};

TEST(BindFront, OneArgumentMemberFunctionWithoutArgumentBound_calledWithArgument_works) {
  Foo fooInstance{3};
  auto boundMemberFunction = bindFront(&Foo::multiply, fooInstance);

  const auto result = boundMemberFunction(2);

  ASSERT_EQ(result, 6);
}

TEST(BindFront, OneArgumentMemberFunctionWithArgumentBound_calledWithoutArgument_works) {
  Foo fooInstance{3};
  auto boundMemberFunction = bindFront(&Foo::multiply, fooInstance, 3);

  const auto result = boundMemberFunction();

  ASSERT_EQ(result, 9);
}

TEST(BindFront, LambdaWithBoundArgument_calledTwice_works) {
  auto lambda = [](std::string arg) { return arg; };
  const std::string arg{"arg"};
  auto boundLambda = bindFront(lambda, arg);

  const auto result1 = boundLambda();
  const auto result2 = boundLambda();

  ASSERT_EQ("arg", result1);
  ASSERT_EQ("arg", result2);
}

TEST(BindFront, NonConstLambda_called_works) {
  auto accumulateFn = [sum = 0](auto increment) mutable { return sum += increment; };

  auto accumulateBy2Fn = bindFront(accumulateFn, 2);

  const auto result1 = accumulateBy2Fn();
  ASSERT_EQ(2, result1);

  const auto result2 = accumulateBy2Fn();
  ASSERT_EQ(4, result2);
}
