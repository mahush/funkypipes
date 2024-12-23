//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

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

// Ensure that transforming multiple arguments out of many works
TEST(
    At,
    arguments01AssignedToCallableTransformingItsArguments_calledWithFourArguments_arguments34ReturnedAt01AndTransformationReturnedAt2) {
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

// Ensure that const lvalue references are preserved in context of multiple selected arguments
TEST(At, multipleArgumentsAssignedToCallableForwardingConstLValueReferences_called_constReferencesArePreserved) {
  // given
  auto forwardingFn = [](const int& arg1, const std::string& arg2) { return std::forward_as_tuple(arg1, arg2); };
  auto fnAt01 = at<0, 1>(forwardingFn);

  // when
  int arg1{1};
  std::string arg2{"two"};
  decltype(auto) result = fnAt01(std::as_const(arg1), std::as_const(arg2));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  arg1 = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  arg2 = "three";
  ASSERT_EQ(std::get<0>(result), 2);
  ASSERT_EQ(std::get<1>(result), "three");
}

// Ensure that lvalue references are preserved in context of multiple selected arguments
TEST(At, multipleArgumentsAssignedToCallableForwardingValueReferences_called_referencesArePreserved) {
  // given
  auto forwardingFn = [](int& arg1, std::string& arg2) { return std::forward_as_tuple(arg1, arg2); };
  auto fnAt01 = at<0, 1>(forwardingFn);

  // when
  int arg1{1};
  std::string arg2{"two"};
  decltype(auto) result = fnAt01(arg1, arg2);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  std::get<0>(result) = 2;
  std::get<1>(result) = "three";
  ASSERT_EQ(arg1, 2);
  ASSERT_EQ(arg2, "three");
}

// Ensure that rvalue references are preserved in context of multiple selected arguments
TEST(At, multipleArgumentsAssignedToCallableForwardingRValueReferences_called_referencesArePreserved) {
  // given
  auto forwardingFn = [](int&& arg1, std::string&& arg2) {
    return std::forward_as_tuple(std::move(arg1), std::move(arg2));
  };
  auto fnAt01 = at<0, 1>(forwardingFn);

  // when
  int arg1{1};              // NOLINT misc-const-correctness: is about to be moved
  std::string arg2{"two"};  // NOLINT misc-const-correctness: is about to be moved
  decltype(auto) result = fnAt01(std::move(arg1), std::move(arg2));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&&, std::string&&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
}

// Ensure that const lvalue references are preserved in context of a single selected argument
TEST(At, singleArgumentAssignedToCallableForwardingConstLValueReferences_called_referencesArePreserved) {
  // given
  auto forwardingFn = [](const int& arg) -> const int& { return arg; };
  auto fnAt0 = at<0>(forwardingFn);

  // when
  int arg{1};
  decltype(auto) result = fnAt0(std::as_const(arg));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  ASSERT_EQ(result, 1);
  arg = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, 2);
}

// Ensure that lvalue references are preserved in context of a single selected argument
TEST(At, singleArgumentAssignedToCallableForwardingLValueReferences_called_referencesArePreserved) {
  // given
  auto forwardingFn = [](int& arg) -> int& { return arg; };
  auto fnAt0 = at<0>(forwardingFn);

  // when
  int arg{1};
  decltype(auto) result = fnAt0(arg);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  ASSERT_EQ(result, 1);
  result = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(arg, 2);
}

// Ensure that rvalue references are preserved in context of a single selected argument
TEST(At, singleArgumentAssignedToCallableForwardingRValueReferences_called_referencesArePreserved) {
  // given
  auto forwardingFn = [](int&& arg) -> int&& { return std::move(arg); };
  auto fnAt0 = at<0>(forwardingFn);

  // when
  int arg{1};  // NOLINT misc-const-correctness: is about to be moved
  decltype(auto) result = fnAt0(std::move(arg));

  // then
  static_assert(std::is_same_v<decltype(result), int&&>);
  ASSERT_EQ(result, 1);
}

// Ensure that const lvalue references are preserved in context of a single not selected argument
TEST(At, noArgumentsAssignedToCallable_calledWithConstLValueReference_referenceIsPreserved) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when
  int arg{1};
  decltype(auto) result = fnAt01(std::as_const(arg));

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  ASSERT_EQ(result, 1);
  arg = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(result, 2);
}

// Ensure that lvalue references are preserved in context of a single not selected argument
TEST(At, noArgumentsAssignedToCallable_calledWithLValueReference_referenceIsPreserved) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when
  int arg{1};
  decltype(auto) result = fnAt01(arg);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  ASSERT_EQ(result, 1);
  result = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  ASSERT_EQ(arg, 2);
}

// Ensure that rvalue references are resolved in context of a single not selected argument
TEST(At, noArgumentsAssignedToCallable_calledWithRValueReference_returnedByValue) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when
  int arg{1};  // NOLINT misc-const-correctness: is about to be moved
  decltype(auto) result = fnAt01(std::move(arg));

  // then
  static_assert(std::is_same_v<decltype(result), int>);
  ASSERT_EQ(result, 1);
}

// Ensure that const lvalue references are preserved in context of multiple not selected arguments
TEST(At, noArgumentsAssignedToCallable_calledWithMultipleConstLValueReferences_referencesArePreserved) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when
  int arg1{1};
  std::string arg2{"two"};
  decltype(auto) result = fnAt01(std::as_const(arg1), std::as_const(arg2));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  arg1 = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  arg2 = "three";
  ASSERT_EQ(std::get<0>(result), 2);
  ASSERT_EQ(std::get<1>(result), "three");
}
// Ensure that lvalue references are preserved in context of multiple not selected arguments
TEST(At, noArgumentsAssignedToCallable_calledWithMultipleLValueReferences_referencesArePreserved) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when
  int arg1{1};
  std::string arg2{"two"};
  decltype(auto) result = fnAt01(arg1, arg2);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  std::get<0>(result) = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: This actually has an effect
  std::get<1>(result) = "three";
  ASSERT_EQ(arg1, 2);
  ASSERT_EQ(arg2, "three");
}

// Ensure that rvalue references are resolved in context of multiple not selected arguments
TEST(At, noArgumentsAssignedToCallable_calledWithMultipleRValueReferences_returnedByValue) {
  // given
  auto forwardingFn = []() {};
  auto fnAt01 = at<>(forwardingFn);

  // when

  decltype(auto) result = fnAt01(int{1}, std::string{"two"});

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, std::string>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
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

// Ensure that consuming multiple arguments out of many works
TEST(At, arguments01AssignedToCallableConsumingItsArgument_calledWithFourArguments_executedAndArguments34Remain) {
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

// Ensure that a generic lambda is supported
TEST(At, genericCallable_calledWithIntegerAndString_works) {
  // given
  auto genericFn = [](auto arg) { return arg + arg; };
  auto fnAt1 = at<1>(genericFn);

  // when
  const auto result1 = fnAt1(1, 2);
  const auto result2 = fnAt1(1, std::string{"two"});

  // then
  ASSERT_EQ(std::get<1>(result1), 4);
  ASSERT_EQ(std::get<1>(result2), "twotwo");
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

    bool& isExecuted_;  // NOLINT misc-non-private-member-variables-in-classes: intended
  };
  auto fnAt0 = at<0>(NonCopyableFn{isExecuted});

  // when
  fnAt0(1);

  // then
  ASSERT_TRUE(isExecuted);
}

// Ensure that a selected argument can be non copyable
TEST(
    At,
    argument1AssignedToCallableForwardingItsNonCopyableArgument_calledWithTwoArgumentsWhereArgument1IsNonCopyable_bothArgumentsReturned) {
  // given
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;

    int value_;  // NOLINT misc-non-private-member-variables-in-classes: intended
  };
  auto forwardingFn = [](NonCopyableArg arg) { return arg; };
  auto fnAt1 = at<1>(forwardingFn);

  // when
  const auto result = fnAt1(std::string{"one"}, NonCopyableArg{2});

  // then
  ASSERT_EQ(std::get<0>(result), "one");
  ASSERT_EQ(std::get<1>(result).value_, 2);
}

// Ensure that a not selected argument can be non copyable
TEST(
    At,
    argument0AssignedToCallableForwardingItsArgument_calledWithTwoArgumentsWhereArgument1IsNonCopyable_bothArgumentsReturned) {
  // given
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;

    int value_;  // NOLINT misc-non-private-member-variables-in-classes: intended
  };
  auto forwardingFn = [](std::string arg) { return arg; };
  auto fnAt0 = at<0>(forwardingFn);

  // when
  const auto result = fnAt0(std::string{"one"}, NonCopyableArg{2});

  // then
  ASSERT_EQ(std::get<0>(result).value_, 2);
  ASSERT_EQ(std::get<1>(result), "one");
}

// Ensure that selecting a single unique type works
TEST(At, argumentsOfTypeIntAssignedToCallableTakingASingleInt_calledWithIntAndString_works) {
  // given
  auto transformingFn = [](int arg) { return arg + 1; };
  auto fnAt0 = at<int>(transformingFn);

  // when
  const auto result = fnAt0(1, "two");

  // then
  ASSERT_EQ(result, std::make_tuple("two", 2));
}

// Ensure that selecting multiple unique types works
TEST(At, argumentsOfTypeIntAndStringAssignedToCallable_calledWithIntAndString_works) {
  // given
  auto transformingFn = [](int arg1, std::string arg2) { return std::to_string(arg1) + arg2; };
  auto fnAt0 = at<int, std::string>(transformingFn);

  // when
  const auto result = fnAt0(1, std::string{"2"});

  // then
  ASSERT_EQ(result, "12");
}

// Ensure that selecting a non unique type works
TEST(At, argumentsOfTypeIntAssignedToCallableTakingTwoInts_calledWithTwoIntAndString_works) {
  // given
  auto transformingFn = [](int arg1, int arg2) { return arg1 + arg2; };
  auto fnAt0 = at<int>(transformingFn);

  // when
  const auto result = fnAt0(1, 2, std::string{"three"});

  // then
  ASSERT_EQ(result, std::make_tuple("three", 3));
}

// Ensure that not matching signature triggers static assert
// TEST(At, argumentsOfTypeIntAssignedToCallableTakingTwoInts_calledWithTwoIntAndString_works2) {
//  // given
//  auto transformingFn = [](int arg) { return arg; };
//  auto fnAt0 = at<std::string>(transformingFn);
//
//  // when
//  const auto result = fnAt0(std::string{"0"});
//}

//// Ensure that selecting an absent type by type raises proper static_assert
// TEST(At, argumentsOfTypeIntAssignedToCallableTakingTwoInts_calledWithTwoIntAndString_works2) {
//   // given
//   auto transformingFn = [](int arg) { return arg; };
//   auto fnAt0 = at<std::string, int>(transformingFn);
//
//   // when
//   const auto result = fnAt0(0);
// }
