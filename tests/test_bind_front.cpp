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

TEST(BindFront, TwoArgumentLambdaWithFirstRValueArgumentBound_calledWithOneRValueArgument_works) {
  auto lambda = [](std::unique_ptr<int> arg1, std::unique_ptr<int> arg2) { return *arg1 + *arg2; };
  auto boundLambda = bindFront(lambda, std::make_unique<int>(10));

  const auto result = boundLambda(std::make_unique<int>(10));

  ASSERT_EQ(result, 20);
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

