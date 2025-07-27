
//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_TESTS_PREDEFINED_EXECUTION_SEMANTICS_MAKE_PIPE_TESTS_HPP
#define FUNKYPIPES_TESTS_PREDEFINED_EXECUTION_SEMANTICS_MAKE_PIPE_TESTS_HPP

#include <gtest/gtest.h>

#include <functional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "utils/move_only_forwarding_fn.hpp"
#include "utils/move_only_struct.hpp"

namespace funkypipes::test::execution_semantics {

static int incrementFn(int value) {
  value++;
  return value;
}

// feature: composing variable amount of callables
template <typename TFn>
void singleCallable_composed_works(TFn makePipeFn) {
  // when
  auto pipe = makePipeFn(incrementFn);

  // then
  auto result = pipe(0);
  ASSERT_EQ(result, 1);
}

template <typename TFn>
void twoCallables_composed_works(TFn makePipeFn) {
  // when
  auto pipe = makePipeFn(incrementFn, incrementFn);

  // then
  auto result = pipe(0);
  ASSERT_EQ(result, 2);
}

template <typename TFn>
void threeCallables_composed_works(TFn makePipeFn) {
  // when
  auto pipe = makePipeFn(incrementFn, incrementFn, incrementFn);

  // then
  auto result = pipe(0);
  ASSERT_EQ(result, 3);
}

// feature: callables - types
static int function(int value) { return value; }
template <typename TFn>
void differentCallableTypes_composed_works(TFn makePipeFn) {
  //
  // given
  //
  auto lambda = [](int value) { return value; };

  struct Functor {
    int operator()(int arg) const { return arg; }
  };
  Functor functor;

  // when
  auto pipe = makePipeFn(lambda, functor, function);

  // then
  auto result = pipe(0);
  ASSERT_EQ(result, 0);
}

// feature: callables - generic callables
template <typename TFn>
void genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted(TFn makePipeFn) {
  // given
  auto generic_lambda = [](auto arg) { return arg; };
  auto pipe = makePipeFn(generic_lambda, generic_lambda);

  // when
  auto result1 = pipe(true);
  auto result2 = pipe("string");

  // then
  ASSERT_EQ(result1, true);
  ASSERT_EQ(result2, "string");
}

template <typename TFn>
void overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted(TFn makePipeFn) {
  //
  // given
  //
  struct OverloadFn1 {
    int operator()(int arg) const { return arg; }
    std::string operator()(std::string arg) const { return arg; }
  };
  struct OverloadFn2 {
    int operator()(int arg) const { return arg; }
    std::string operator()(std::string arg) const { return arg; }
  };

  auto pipe = makePipeFn(OverloadFn1{}, OverloadFn2{});

  // when
  auto result1 = pipe(0);
  auto result2 = pipe(std::string{"0"});

  // then
  EXPECT_EQ(result1, 0);
  EXPECT_EQ(result2, "0");
}

// feature: callables - move only
template <typename TFn>
void nonCopyableCallables_composed_works(TFn makePipeFn) {
  // when
  auto pipe = makePipeFn(MoveOnlyForwardingFn{}, MoveOnlyForwardingFn{}, MoveOnlyForwardingFn{});

  // then
  EXPECT_EQ(0, pipe(0));
}

// feature: callables - assignable to std::function
template <typename TFn>
void genericPipe_assignedToStdFunctions_works(TFn makePipeFn) {
  // given
  auto generic_lambda = [](auto arg) { return arg; };
  auto pipe = makePipeFn(generic_lambda, generic_lambda);

  // when
  std::function<int(int)> function_a = pipe;
  std::function<std::string(std::string)> function_b = pipe;

  // then
  ASSERT_EQ(1, function_a(1));
  ASSERT_EQ("1", function_b("1"));
}

// feature: callables - propagate exceptions
template <typename TFn>
void pipeComposed_callableThrows_exceptionPropagatedToCaller(TFn makePipeFn) {
  // given
  auto lambda_1 = [](bool) -> int { return 0; };
  auto lambda_2 = [](int) -> bool {
    throw std::exception{};
    return false;
  };
  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when and then
  EXPECT_THROW(pipe(false), std::exception);
}

// feature: data - value categories
template <typename TFn>
void compositionWithValueArgument_calledWithLValue_works(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  int argument{0};
  auto result = pipe(argument);

  // then
  ASSERT_EQ(result, "0");
}

template <typename TFn>
void compositionWithValueArgument_calledWithRValue_works(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](int value) -> int { return value; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  int argument{0};
  auto result = pipe(std::move(argument));

  // then
  ASSERT_EQ(result, "0");
}

template <typename TFn>
void callablesForwardingConstRefererence_composed_constReferencesArePreserved(TFn makePipeFn) {
  // given
  auto lambda = [](const int& value) -> const int& { return value; };
  auto pipe = makePipeFn(lambda, lambda);

  // when
  int argument{0};
  decltype(auto) result = pipe(argument);

  // then
  static_assert(std::is_same_v<decltype(result), const int&>);
  ASSERT_EQ(result, 0);
  argument++;
  ASSERT_EQ(result, 1);
}

template <typename TFn>
void callablesForwardingRefererence_composed_referencesArePreserved(TFn makePipeFn) {
  // given
  auto lambda = [](int& value) -> int& { return value; };
  auto pipe = makePipeFn(lambda, lambda);

  // when
  int argument{1};
  decltype(auto) result = pipe(argument);

  // then
  static_assert(std::is_same_v<decltype(result), int&>);
  ASSERT_EQ(result, 1);
  result++;
  ASSERT_EQ(argument, 2);
}

template <typename TFn>
void callablesForwardingMultipleConstRefererences_composed_constReferencesArePreserved(TFn makePipeFn) {
  // given
  auto forwardingFn = [](const int& arg1, const std::string& arg2) { return std::forward_as_tuple(arg1, arg2); };
  auto pipe = makePipeFn(forwardingFn, forwardingFn);

  // when
  int arg1 = 1;
  std::string arg2 = "two";
  decltype(auto) result = pipe(std::as_const(arg1), std::as_const(arg2));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  arg1 = 2;
  arg2 = "three";
  ASSERT_EQ(std::get<0>(result), 2);
  ASSERT_EQ(std::get<1>(result), "three");
}

template <typename TFn>
void callablesForwardingMultipleRefererences_composed_referencesArePreserved(TFn makePipeFn) {
  // given
  auto forwardingFn = [](int& arg1, std::string& arg2) { return std::forward_as_tuple(arg1, arg2); };
  auto pipe = makePipeFn(forwardingFn, forwardingFn);

  // when
  int arg1 = 1;
  std::string arg2 = "two";
  decltype(auto) result = pipe(arg1, arg2);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, std::string&>>);
  ASSERT_EQ(std::get<0>(result), 1);
  ASSERT_EQ(std::get<1>(result), "two");
  std::get<0>(result) = 2;
  std::get<1>(result) = "three";
  ASSERT_EQ(arg1, 2);
  ASSERT_EQ(arg2, "three");
}

// feature: data - move only
template <typename TFn>
void callablesWithNonCopyableArguments_composed_works(TFn makePipeFn) {
  // given
  auto lambda = [](MoveOnlyStruct arg) { return arg; };

  // when
  auto pipe = makePipeFn(lambda, lambda, lambda);

  // then
  auto res = pipe(MoveOnlyStruct{0});
  EXPECT_EQ(res.value_, 0);
}

// feature: data - any number of arguments
template <typename TFn>
void compositionWithSingleArgument_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](bool flag) -> int { return flag ? 7 : 0; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };
  auto lambda_3 = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makePipeFn(lambda_1, lambda_2, lambda_3);

  // when
  bool flag = true;
  auto result = pipe(flag);

  // then
  ASSERT_EQ(result, "77");
}

template <typename TFn>
void compositionWithMultipleArguments_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  auto result = pipe(1, 2);

  // then
  ASSERT_EQ(result, "3");
}

template <typename TFn>
void compositionWithTupleArgument_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  auto result = pipe(std::make_tuple(3, 4));

  // then
  ASSERT_EQ(result, "7");
}

template <typename TFn>
void compositionWithTuplePassthrough_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](bool flag) -> std::tuple<int, std::string> { return {flag, "2"}; };
  auto lambda_2 = [](int arg1, const std::string& arg2) -> std::string { return std::to_string(arg1) + arg2; };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  bool flag = true;
  auto result = pipe(flag);

  // then
  ASSERT_EQ(result, "12");
}

template <typename TFn>
void compositionWithTupleResult_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = [](bool flag) -> bool { return static_cast<int>(flag); };
  auto lambda_2 = [](int value) -> std::tuple<int, std::string> { return {value, std::to_string(value)}; };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // given
  auto [result_1, result_2] = pipe(true);

  // then
  ASSERT_EQ(result_1, 1);
  ASSERT_EQ(result_2, "1");
}

template <typename TFn>
void compositionWithoutArguments_called_isExecuted(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = []() -> int { return 0; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makePipeFn(lambda_1, lambda_2);

  // when
  auto result = pipe();

  // then
  ASSERT_EQ(result, "0");
}

// feature: data - void return type
template <typename TFn>
void callablesReturningVoid_composedAsPipe_pipeReturnsVoid(TFn makePipeFn) {
  //
  // given
  //
  auto lambda_1 = []() -> void {};
  auto lambda_2 = []() -> void {};

  // when
  auto pipe = makePipeFn(lambda_1, lambda_2);

  // then
  pipe();
  using ResultType = std::invoke_result_t<decltype(pipe)>;
  static_assert(std::is_void_v<ResultType>);
}

}  // namespace funkypipes::test::execution_semantics

#endif  // FUNKYPIPES_TESTS_PREDEFINED_EXECUTION_SEMANTICS_MAKE_PIPE_TESTS_HPP
