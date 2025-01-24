//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <optional>

#include "funkypipes/funky_void.hpp"
#include "funkypipes/make_pipe.hpp"
#include "predefined/execution_semantics/make_pipe_tests.hpp"

namespace funkypipes::test {

using namespace execution_semantics;

auto makePipeFn = [](auto&&... args) { return makePipe(std::forward<decltype(args)>(args)...); };

// feature: composing variable amount of callables
TEST(MakePipe, singleCallable_composed_works) { ASSERT_NO_FATAL_FAILURE(singleCallable_composed_works(makePipeFn)); }
TEST(MakePipe, twoCallables_composed_works) { ASSERT_NO_FATAL_FAILURE(twoCallables_composed_works(makePipeFn)); }
TEST(MakePipe, threeCallables_composed_works) { ASSERT_NO_FATAL_FAILURE(threeCallables_composed_works(makePipeFn)); }

// feature: callables - types
TEST(MakePipe, differentCallableTypes_composed_works) {
  ASSERT_NO_FATAL_FAILURE(differentCallableTypes_composed_works(makePipeFn));
}

// feature: callables - generic callables
TEST(MakePipe, genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted(makePipeFn));
}
TEST(MakePipe, overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted(makePipeFn));
}

// feature: callables - move only
TEST(MakePipe, nonCopyableCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(nonCopyableCallables_composed_works(makePipeFn));
}

// feature: callables - assignable to std::function
TEST(MakePipe, genericPipe_assignedToStdFunctions_works) {
  ASSERT_NO_FATAL_FAILURE(genericPipe_assignedToStdFunctions_works(makePipeFn));
}

// feature: callables - propagate exceptions
TEST(MakePipe, pipeComposed_callableThrows_exceptionPropagatedToCaller) {
  ASSERT_NO_FATAL_FAILURE(pipeComposed_callableThrows_exceptionPropagatedToCaller(makePipeFn));
}

// feature: data - value categories
TEST(MakePipe, compositionWithValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithLValue_works(makePipeFn));
}
TEST(MakePipe, compositionWithValueArgument_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithRValue_works(makePipeFn));
}

TEST(MakePipe, callablesForwardingRefererence_composed_referencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingRefererence_composed_referencesArePreserved(makePipeFn));
}

TEST(MakePipe, callablesForwardingConstRefererence_composed_constReferencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingConstRefererence_composed_constReferencesArePreserved(makePipeFn));
}

TEST(MakePipe, callablesForwardingMultipleConstRefererences_composed_constReferencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(
      callablesForwardingMultipleConstRefererences_composed_constReferencesArePreserved(makePipeFn));
}

TEST(MakePipe, callablesForwardingMultipleRefererences_composed_referencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingMultipleRefererences_composed_referencesArePreserved(makePipeFn));
}

// feature: data - move only
TEST(MakePipe, callablesWithNonCopyableArguments_composed_works) {
  ASSERT_NO_FATAL_FAILURE(callablesWithNonCopyableArguments_composed_works(makePipeFn));
}

// feature: data - any number of arguments
TEST(MakePipe, compositionWithSingleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithSingleArgument_called_isExecuted(makePipeFn));
}

TEST(MakePipe, compositionWithMultipleArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithMultipleArguments_called_isExecuted(makePipeFn));
}

TEST(MakePipe, compositionWithTupleArgument_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTupleArgument_called_isExecuted(makePipeFn));
}

TEST(MakePipe, compositionWithTuplePassthrough_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTuplePassthrough_called_isExecuted(makePipeFn));
}

TEST(MakePipe, failableCompositionWithTuplePassthrough_called_isExecuted) {
  auto lambda_1 = [](bool flag) -> std::optional<std::tuple<int, std::string>> {
    return std::tuple<int, std::string>({flag, "2"});
  };
  auto lambda_2 = [](int arg1, const std::string& arg2) -> std::string { return std::to_string(arg1) + arg2; };

  auto pipe = makePipe(lambda_1, andThen(lambda_2));
  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "12");
}

TEST(MakePipe, compositionWithTupleResult_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithTupleResult_called_isExecuted(makePipeFn));
}

TEST(MakePipe, compositionWithoutArguments_called_isExecuted) {
  ASSERT_NO_FATAL_FAILURE(compositionWithoutArguments_called_isExecuted(makePipeFn));
}

// feature: data - void return type
TEST(MakePipe, callablesReturningVoid_composedAsPipe_pipeReturnsVoid) {
  ASSERT_NO_FATAL_FAILURE(callablesReturningVoid_composedAsPipe_pipeReturnsVoid(makePipeFn));
}

TEST(MakePipe, breakablePipeReturningVoid_breaks_pipeReturnsOptionalVoid) {
  auto breaking_lambda = []() -> std::optional<int> { return std::nullopt; };
  auto void_returning_lambda = [](int) -> void {};

  auto pipe = makePipe(breaking_lambda, andThen(void_returning_lambda));

  std::optional<FunkyVoid> result = pipe();
  ASSERT_FALSE(result.has_value());
}

// feature: std::optional support
static int function_1(bool flag) { return flag ? 7 : 0; }
static std::string function_2(int value) { return std::to_string(value); }
static std::string function_3(std::string string) { return string + string; }

TEST(MakePipe, pipeAcceptingOptional_calledWithLValueOptional_isExecuted) {
  auto pipe = makePipe(andThen(function_1), andThen(function_2), andThen(function_3));

  std::optional<bool> flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakePipe, pipeAcceptingOptional_calledWithRValueOptional_isExecuted) {
  auto pipe = makePipe(andThen(function_1), andThen(function_2), andThen(function_3));

  auto result = pipe(std::optional<bool>{true});
  ASSERT_EQ(result, "77");
}

// feature: chain breaking - skipping on nullopt
TEST(MakePipe, breakablePipe_intermediateLambdaReturnsNullopt_theChainBreaks) {
  auto breaking_lambda = [](bool) -> std::optional<int> { return std::nullopt; };
  auto subsequent_lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makePipe(breaking_lambda, andThen(subsequent_lambda), andThen(subsequent_lambda));

  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(true));
  EXPECT_FALSE(res.has_value());
}

TEST(MakePipe, breakablePipe_intermediateFunkyVoidReturningLambdaReturnsNullopt_theChainBreaks) {
  auto breaking_lambda = []() -> std::optional<FunkyVoid> { return std::nullopt; };
  auto subsequent_lambda = []() -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makePipe(breaking_lambda, andThen(subsequent_lambda));

  std::optional<int> result;
  EXPECT_NO_THROW(result = pipe());
  EXPECT_FALSE(result.has_value());
}

TEST(MakePipe, lambdasComposition_calledWithNullopt_notingIsExecuted) {
  auto lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makePipe(andThen(lambda), andThen(lambda), andThen(lambda));

  std::optional<int> arg{std::nullopt};
  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(arg));
  EXPECT_FALSE(res.has_value());
}

// feature: chain breaking - nested pipes
TEST(MakePipe, nestedFailingPipeReturningVoid_composedAsPipe_topLevelPipeFails) {
  auto failable_callable = [](bool fail) -> std::optional<int> { return fail ? std::nullopt : std::make_optional(1); };
  auto void_returning_callable = [](int) -> void {};
  auto failable_void_returning_pipe = makePipe(failable_callable, andThen(void_returning_callable));

  auto string_returning_callable = []() -> std::string { return "result"; };

  auto failable_top_level_pipe = makePipe(failable_void_returning_pipe, andThen(string_returning_callable));

  {
    std::optional<std::string> result = failable_top_level_pipe(false);
    ASSERT_EQ(result, "result");
  }
  {
    std::optional<std::string> result = failable_top_level_pipe(true);
    ASSERT_FALSE(result.has_value());
  }
}

}  // namespace funkypipes::test
