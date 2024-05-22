//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/make_failable_pipe.hpp"
#include "predefined/execution_semantics/make_pipe_tests.hpp"

namespace funkypipes::test {

using namespace execution_semantics;

auto makeFailablePipeFn = [](auto&&... args) { return makeFailablePipe(std::forward<decltype(args)>(args)...); };

TEST(MakeFailablePipe, singleCallable_composed_works) {
  ASSERT_NO_FATAL_FAILURE(singleCallable_composed_works(makeFailablePipeFn));
}
TEST(MakeFailablePipe, twoCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(twoCallables_composed_works(makeFailablePipeFn));
}
TEST(MakeFailablePipe, threeCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(threeCallables_composed_works(makeFailablePipeFn));
}

// feature: callables - types
TEST(MakeFailablePipe, differentCallableTypes_composed_works) {
  ASSERT_NO_FATAL_FAILURE(differentCallableTypes_composed_works(makeFailablePipeFn));
}

// feature: callables - generic callables
TEST(MakeFailablePipe, genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted(makeFailablePipeFn));
}
TEST(MakeFailablePipe, overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted(makeFailablePipeFn));
}

// feature: callables - move only
TEST(MakeFailablePipe, nonCopyableCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(nonCopyableCallables_composed_works(makeFailablePipeFn));
}

// feature: callables - assignable to std::function
TEST(MakeFailablePipe, genericPipe_assignedToStdFunctions_works) {
  auto generic_lambda = [](auto arg) { return arg; };

  auto pipe = makeFailablePipe(generic_lambda, generic_lambda);
  std::function<std::optional<int>(int)> function_a = pipe;
  std::function<std::optional<std::string>(std::string)> function_b = pipe;

  ASSERT_EQ(1, function_a(1));
  ASSERT_EQ("1", function_b("1"));
}

// feature: callables - propagate exceptions
TEST(MakeFailablePipe, pipeComposed_callableThrows_exceptionPropagatedToCaller) {
  ASSERT_NO_FATAL_FAILURE(pipeComposed_callableThrows_exceptionPropagatedToCaller(makeFailablePipeFn));
}

// feature: data - value categories
TEST(MakeFailablePipe, compositionWithValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithLValue_works(makeFailablePipeFn));
}
TEST(MakeFailablePipe, compositionWithValueArgument_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithRValue_works(makeFailablePipeFn));
}

TEST(MakeFailablePipe, callablesForwardingRefererence_composed_referencesArePreserved) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  result.value()++;
  ASSERT_EQ(argument, 2);
}

TEST(MakeFailablePipe, callablesForwardingConstRefererence_composed_constReferencesArePreserved) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<const int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  argument++;
  ASSERT_EQ(result, 2);
}

// feature: data - move only
TEST(MakeFailablePipe, callablesWithNonCopyableArguments_composed_works) {
  struct NonCopyableArg {
    explicit NonCopyableArg(int arg) : m_arg{arg} {}
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
    int m_arg;
  };
  auto lambda = [](NonCopyableArg arg) { return arg; };
  auto lambda_optional = [](NonCopyableArg arg) { return std::make_optional<NonCopyableArg>(std::move(arg)); };

  auto pipe = makeFailablePipe(lambda, lambda_optional, lambda);
  auto res = pipe(NonCopyableArg{0});

  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(0, res.value().m_arg);
}

// feature: data - any number of arguments
TEST(MakeFailablePipe, compositionWithSingleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithSingleArgument_called_isExecuted(makeFailablePipeFn));
}

TEST(MakeFailablePipe, compositionWithMultipleArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithMultipleArguments_called_isExecuted(makeFailablePipeFn));
}

TEST(MakeFailablePipe, compositionWithTupleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTupleArgument_called_isExecuted(makeFailablePipeFn));
}

TEST(MakeFailablePipe, compositionWithTuplePassthrough_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTuplePassthrough_called_isExecuted(makeFailablePipeFn));
}

TEST(MakefailablePipe, failableCompositionWithTuplePassthrough_called_isExecuted) {
  auto lambda_1 = [](bool flag) -> std::optional<std::tuple<int, std::string>> {
    return std::tuple<int, std::string>({flag, "2"});
  };
  auto lambda_2 = [](int arg1, const std::string& arg2) -> std::string { return std::to_string(arg1) + arg2; };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);
  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "12");
}

TEST(MakeFailablePipe, failableCompositionWithTupleResult_called_isExecuted) {
  auto lambda_1 = [](bool flag) -> bool { return static_cast<int>(flag); };
  auto lambda_2 = [](int value) -> std::tuple<int, std::string> { return {value, std::to_string(value)}; };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  bool flag = true;
  auto tuple_result = pipe(flag);

  ASSERT_TRUE(tuple_result.has_value());
  auto [result_1, result_2] = tuple_result.value();
  ASSERT_EQ(result_1, 1);
  ASSERT_EQ(result_2, "1");
}

TEST(MakeFailablePipe, compositionWithoutArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithoutArguments_called_isExecuted(makeFailablePipeFn));
}

// feature: std::optional support
static int function_1(bool flag) { return flag ? 7 : 0; };
static std::string function_2(int value) { return std::to_string(value); };
static std::string function_3(std::string string) { return string + string; };

TEST(MakeFailablePipe, pipeAcceptingOptional_calledWithLValueOptional_isExecuted) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  std::optional<bool> flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, pipeAcceptingOptional_calledWithRValueOptional_isExecuted) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  auto result = pipe(std::optional<bool>{true});
  ASSERT_EQ(result, "77");
}

// feature: chain breaking
TEST(MakeFailablePipe, lambdasComposition_intermediateLambdaReturnsNullopt_theChainBreaks) {
  auto breaking_lambda = [](bool) -> std::optional<int> { return std::nullopt; };
  auto subsequent_lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeFailablePipe(breaking_lambda, subsequent_lambda, subsequent_lambda);

  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(true));
  EXPECT_FALSE(res.has_value());
}

TEST(MakeFailablePipe, lambdasComposition_calledWithNullopt_notingIsExecuted) {
  auto lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeFailablePipe(lambda, lambda, lambda);

  std::optional<int> arg{std::nullopt};
  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(arg));
  EXPECT_FALSE(res.has_value());
}

// TEST(MakeFailablePipe, voidReturnTypeCallable_compose_triggersStaticAssert)
// {
//   auto lambda_1 = [](bool) -> int { return 0; };
//   auto lambda_2 = [](int) -> void {};
//
//   auto pipe = makeFailablePipe(lambda_1, lambda_2);
//
//   pipe(false);
// }

// TEST(MakeFailablePipe,
// missmatchingArgumentTypeCompostion_compose_triggersStaticAssert) {
//   auto lambda_returning_bool = [](bool) -> bool { return false; };
//   auto lambda_taking_string = [](std::string) -> int { return 0; };
//
//   auto pipe = makeFailablePipe(lambda_returning_bool, lambda_taking_string);
//
//   pipe(false);
// }

}  // namespace funkypipes::test
