//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>

#include "funkypipes/at.hpp"

using funkypipes::at;

// Ensure that transforming the only argument works
TEST(At, argument0AssignedToCallableTransformingItsArgument_calledWithSingleArguments_transformationReturned) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto fnAt0 = at<0>(transformingFn);

  // when
  const auto result = fnAt0(0);

  // then
  ASSERT_EQ(result, 1);
}

// Ensure that consuming the only argument works
TEST(At, argument0AssignedToCallableConsumingItsArgument_calledWithSingleArguments_executedAndVoidReturned) {
  // given
  bool fnExecuted{false};
  auto consumingFn = [&fnExecuted](int) { fnExecuted = true; };

  auto fnAt0 = at<0>(consumingFn);

  // when
  fnAt0(2);

  // then
  ASSERT_TRUE(fnExecuted);

  using ResultType = std::invoke_result_t<decltype(fnAt0), int>;
  static_assert(std::is_same_v<ResultType, void>);
}

// Ensure that transforming one argument out of many works
TEST(
    At,
    argument1AssignedToCallableTransformingItsArgument_calledWithTwoArguments_argument0RemainsAndTransformationReturnedAt1) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto fnAt1 = at<1>(transformingFn);

  // when
  const auto result = fnAt1(std::string{"one"}, 2);

  // then
  ASSERT_EQ(std::get<0>(result), "one");
  ASSERT_EQ(std::get<1>(result), 3);
}

// Ensure that consuming one argument out of many works
TEST(At, argument1AssignedToCallableConsumingItsArguments_calledWithTwoArguments_executedAndArgument0Remains) {
  // given
  bool fnExecuted{false};
  auto consumingFn = [&fnExecuted](int) { fnExecuted = true; };

  auto fnAt1 = at<1>(consumingFn);

  // when
  const auto result = fnAt1(std::string{"one"}, 2);

  // then
  ASSERT_TRUE(fnExecuted);
  ASSERT_EQ(result, "one");
}

// Ensure that transforming multiple arguments out of many works
TEST(
    At,
    arguments01AssignedToCallableTransformingItsArguments_calledWithFourArguments_arguments12ReturnedAt01AndTransformationReturnedAt2) {
  // given
  auto transformingFn = [](int arg1, std::string arg2) { return std::to_string(arg1) + arg2; };

  auto fnAt01 = at<0, 1>(transformingFn);

  // when
  const auto result = fnAt01(1, std::string{"two"}, 3.0, std::string{"four"});

  // then
  ASSERT_EQ(std::get<0>(result), 3.0);
  ASSERT_EQ(std::get<1>(result), "four");
  ASSERT_EQ(std::get<2>(result), "1two");
}

// Ensure that consuming multiple arguments out of many works
TEST(At, arguments01AssignedToCallableConsumingItsArgument_calledWithFourArguments_executedAndArguments12Remain) {
  // given
  bool fnExecuted{false};
  auto consumingFn = [&fnExecuted](int, std::string) { fnExecuted = true; };

  auto fnAt01 = at<0, 1>(consumingFn);

  // when
  auto result = fnAt01(1, std::string{"two"}, 3.0, std::string{"four"});

  // then
  ASSERT_TRUE(fnExecuted);
  ASSERT_EQ(std::get<0>(result), 3.0);
  ASSERT_EQ(std::get<1>(result), "four");
}

// Ensure that move only fn is accepted
TEST(At, NonCopyableFunctorDecorated_called_isExecuted) {
  // given
  bool isExecuted{false};
  struct NonCopyableFn {
    explicit NonCopyableFn(bool& isExecuted) : isExecuted_{isExecuted} {}

    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    void operator()(int) const { isExecuted_ = true; }

    bool& isExecuted_;
  };
  auto fnAt0 = at<0>(NonCopyableFn{isExecuted});

  // when
  fnAt0(1);

  // then
  ASSERT_TRUE(isExecuted);
}
