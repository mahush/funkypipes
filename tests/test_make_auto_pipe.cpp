//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <functional>
#include <optional>

#include "funkypipes/funky_void.hpp"
#include "funkypipes/make_auto_pipe.hpp"
#include "predefined/execution_semantics/make_pipe_tests.hpp"

namespace funkypipes::test {

using namespace execution_semantics;

auto makeAutoPipeFn = [](auto&&... args) { return makeAutoPipe(std::forward<decltype(args)>(args)...); };

// feature: composing variable amount of callables
TEST(MakeAutoPipe, singleCallable_composed_works) {
  ASSERT_NO_FATAL_FAILURE(singleCallable_composed_works(makeAutoPipeFn));
}
TEST(MakeAutoPipe, twoCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(twoCallables_composed_works(makeAutoPipeFn));
}
TEST(MakeAutoPipe, threeCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(threeCallables_composed_works(makeAutoPipeFn));
}

// feature: callables - types
TEST(MakeAutoPipe, differentCallableTypes_composed_works) {
  ASSERT_NO_FATAL_FAILURE(differentCallableTypes_composed_works(makeAutoPipeFn));
}

// feature: callables - generic callables
TEST(MakeAutoPipe, genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted(makeAutoPipeFn));
}
TEST(MakeAutoPipe, overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted(makeAutoPipeFn));
}

// feature: callables - move only
TEST(MakeAutoPipe, nonCopyableCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(nonCopyableCallables_composed_works(makeAutoPipeFn));
}

// feature: callables - assignable to std::function
TEST(MakeAutoPipe, genericPipe_assignedToStdFunctions_works) {
  ASSERT_NO_FATAL_FAILURE(genericPipe_assignedToStdFunctions_works(makeAutoPipeFn));
}

// feature: callables - propagate exceptions
TEST(MakeAutoPipe, pipeComposed_callableThrows_exceptionPropagatedToCaller) {
  ASSERT_NO_FATAL_FAILURE(pipeComposed_callableThrows_exceptionPropagatedToCaller(makeAutoPipeFn));
}

// feature: data - value categories
TEST(MakeAutoPipe, compositionWithValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithLValue_works(makeAutoPipeFn));
}
TEST(MakeAutoPipe, compositionWithValueArgument_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithRValue_works(makeAutoPipeFn));
}

TEST(MakeAutoPipe, callablesForwardingRefererence_composed_referencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingRefererence_composed_referencesArePreserved(makeAutoPipeFn));
}

TEST(MakeAutoPipe, callablesForwardingConstRefererence_composed_constReferencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingConstRefererence_composed_constReferencesArePreserved(makeAutoPipeFn));
}

// feature: data - move only
TEST(MakeAutoPipe, callablesWithNonCopyableArguments_composed_works) {
  ASSERT_NO_FATAL_FAILURE(callablesWithNonCopyableArguments_composed_works(makeAutoPipeFn));
}

// feature: data - any number of arguments
TEST(MakeAutoPipe, compositionWithSingleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithSingleArgument_called_isExecuted(makeAutoPipeFn));
}

TEST(MakeAutoPipe, compositionWithMultipleArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithMultipleArguments_called_isExecuted(makeAutoPipeFn));
}

TEST(MakeAutoPipe, compositionWithTupleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTupleArgument_called_isExecuted(makeAutoPipeFn));
}

TEST(MakeAutoPipe, compositionWithTuplePassthrough_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTuplePassthrough_called_isExecuted(makeAutoPipeFn));
}

TEST(MakeAutoPipe, failableCompositionWithTuplePassthrough_called_isExecuted) {
  auto lambda_1 = [](bool flag) -> std::optional<std::tuple<int, std::string>> {
    return std::tuple<int, std::string>({flag, "2"});
  };
  auto lambda_2 = [](int arg1, const std::string& arg2) -> std::string { return std::to_string(arg1) + arg2; };

  auto pipe = makeAutoPipe(lambda_1, lambda_2);
  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "12");
}

TEST(MakeAutoPipe, failableCompositionWithTupleResult_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTupleResult_called_isExecuted(makeAutoPipeFn));
}

TEST(MakeAutoPipe, compositionWithoutArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithoutArguments_called_isExecuted(makeAutoPipeFn));
}

// feature: data - void return type
TEST(MakeAutoPipe, callablesReturningVoid_composedAsPipe_pipeReturnsVoid) {
  ASSERT_NO_FATAL_FAILURE(callablesReturningVoid_composedAsPipe_pipeReturnsVoid(makeAutoPipeFn));
}

TEST(MakeAutoPipe, breakablePipeReturningVoid_breaks_pipeReturnsOptionalVoid) {
  auto breaking_lambda = []() -> std::optional<int> { return std::nullopt; };
  auto void_returning_lambda = [](int) -> void {};

  auto pipe = makeAutoPipe(breaking_lambda, void_returning_lambda);

  std::optional<FunkyVoid> result = pipe();
  ASSERT_FALSE(result.has_value());
}

// feature: std::optional support
static int function_1(bool flag) { return flag ? 7 : 0; };
static std::string function_2(int value) { return std::to_string(value); };
static std::string function_3(std::string string) { return string + string; };

TEST(MakeAutoPipe, pipeAcceptingOptional_calledWithLValueOptional_isExecuted) {
  auto pipe = makeAutoPipe(function_1, function_2, function_3);

  std::optional<bool> flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeAutoPipe, pipeAcceptingOptional_calledWithRValueOptional_isExecuted) {
  auto pipe = makeAutoPipe(function_1, function_2, function_3);

  std::optional<bool> flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeAutoPipe, pipeForwardingRefererence_calledWithOptionalReference_referenceIsPreserved) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeAutoPipe(lambda, lambda);

  int underlying_value{1};
  std::optional<std::reference_wrapper<int>> argument{underlying_value};
  auto result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  result.value()++;
  ASSERT_EQ(argument, 2);
}

TEST(MakeAutoPipe, pipeForwardingConstRefererence_calledWithOptionalConstReference_referenceIsPreserved) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto pipe = makeAutoPipe(lambda, lambda);

  int underlying_value{1};
  std::optional<std::reference_wrapper<const int>> argument{underlying_value};
  auto result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  underlying_value++;
  ASSERT_EQ(result, 2);
}

TEST(MakeAutoPipe, pipeWithNonCopyableOptionalArguments_composed_works) {
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

  auto pipe = makeAutoPipe(lambda, lambda_optional, lambda);
  auto res = pipe(NonCopyableArg{0});

  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(0, res.value().m_arg);
}

// feature: chain breaking - skipping on nullopt
TEST(MakeAutoPipe, lambdasComposition_intermediateLambdaReturnsNullopt_theChainBreaks) {
  auto breaking_lambda = [](bool) -> std::optional<int> { return std::nullopt; };
  auto subsequent_lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeAutoPipe(breaking_lambda, subsequent_lambda, subsequent_lambda);

  std::optional<int> result;
  EXPECT_NO_THROW(result = pipe(true));
  EXPECT_FALSE(result.has_value());
}

TEST(MakeAutoPipe, breakablePipe_intermediateFunkyVoidReturningLambdaReturnsNullopt_theChainBreaks) {
  auto breaking_lambda = []() -> std::optional<FunkyVoid> { return std::nullopt; };
  auto subsequent_lambda = []() -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeAutoPipe(breaking_lambda, subsequent_lambda);

  std::optional<int> result;
  EXPECT_NO_THROW(result = pipe());
  EXPECT_FALSE(result.has_value());
}

TEST(MakeAutoPipe, lambdasComposition_calledWithNullopt_notingIsExecuted) {
  auto lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeAutoPipe(lambda, lambda, lambda);

  std::optional<int> arg{std::nullopt};
  std::optional<int> result;
  EXPECT_NO_THROW(result = pipe(arg));
  EXPECT_FALSE(result.has_value());
}

// feature: chain breaking - nested pipes
TEST(MakeAutoPipe, nestedFailingPipeReturningVoid_composedAsPipe_topLevelPipeFails) {
  auto failable_callable = [](bool fail) -> std::optional<int> { return fail ? std::nullopt : std::make_optional(1); };
  auto void_returning_callable = [](int) -> void {};
  auto failable_void_returning_pipe = makeAutoPipe(failable_callable, void_returning_callable);

  auto string_returning_callable = []() -> std::string { return "result"; };

  auto failable_top_level_pipe = makeAutoPipe(failable_void_returning_pipe, string_returning_callable);

  {
    std::optional<std::string> result = failable_top_level_pipe(false);
    ASSERT_EQ(result, "result");
  }
  {
    std::optional<std::string> result = failable_top_level_pipe(true);
    ASSERT_FALSE(result.has_value());
  }
}

// TEST(MakeAutoPipe,
// missmatchingArgumentTypeCompostion_compose_triggersStaticAssert) {
//   auto lambda_returning_bool = [](bool) -> bool { return false; };
//   auto lambda_taking_string = [](std::string) -> int { return 0; };
//
//   auto pipe = makeAutoPipe(lambda_returning_bool, lambda_taking_string);
//
//   pipe(false);
// }

}  // namespace funkypipes::test
