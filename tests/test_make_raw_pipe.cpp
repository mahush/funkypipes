//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_raw_pipe.hpp"
#include "predefined/execution_semantics/make_pipe_tests.hpp"

namespace funkypipes::test {

using namespace execution_semantics;

auto makeRawPipeFn = [](auto&&... args) { return details::makeRawPipe(std::forward<decltype(args)>(args)...); };

// feature: composing variable amount of callables
TEST(MakeRawPipe, singleCallable_composed_works) {
  ASSERT_NO_FATAL_FAILURE(singleCallable_composed_works(makeRawPipeFn));
}
TEST(MakeRawPipe, twoCallables_composed_works) { ASSERT_NO_FATAL_FAILURE(twoCallables_composed_works(makeRawPipeFn)); }
TEST(MakeRawPipe, threeCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(threeCallables_composed_works(makeRawPipeFn));
}

// feature: callables - types
TEST(MakeRawPipe, differentCallableTypes_composed_works) {
  ASSERT_NO_FATAL_FAILURE(differentCallableTypes_composed_works(makeRawPipeFn));
}

// feature: callables - generic callables
TEST(MakeRawPipe, genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(genericLambdaPipe_calledWithDifferentTypes_typeSpecificChainExecuted(makeRawPipeFn));
}
TEST(MakeRawPipe, overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted) {
  ASSERT_NO_FATAL_FAILURE(overloadedFunctorsPipe_calledWithEachType_typeSpecificChainExecuted(makeRawPipeFn));
}

// feature: callables - move only
TEST(MakeRawPipe, nonCopyableCallables_composed_works) {
  ASSERT_NO_FATAL_FAILURE(nonCopyableCallables_composed_works(makeRawPipeFn));
}

// feature: callables - assignable to std::function
TEST(MakeRawPipe, genericPipe_assignedToStdFunctions_works) {
  ASSERT_NO_FATAL_FAILURE(genericPipe_assignedToStdFunctions_works(makeRawPipeFn));
}

// feature: callables - propagate exceptions
TEST(MakeRawPipe, pipeComposed_callableThrows_exceptionPropagatedToCaller) {
  ASSERT_NO_FATAL_FAILURE(pipeComposed_callableThrows_exceptionPropagatedToCaller(makeRawPipeFn));
}

// feature: data - value categories
TEST(MakeRawPipe, compositionWithValueArgument_calledWithLValue_works) {
  ASSERT_NO_FATAL_FAILURE(pipeComposed_callableThrows_exceptionPropagatedToCaller(makeRawPipeFn));
}
TEST(MakeRawPipe, compositionWithValueArgument_calledWithRValue_works) {
  ASSERT_NO_FATAL_FAILURE(compositionWithValueArgument_calledWithRValue_works(makeRawPipeFn));
}
TEST(MakeRawPipe, callablesForwardingConstRefererence_composed_constReferencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingConstRefererence_composed_constReferencesArePreserved(makeRawPipeFn));
}
TEST(MakeRawPipe, callablesForwardingRefererence_composed_referencesArePreserved) {
  ASSERT_NO_FATAL_FAILURE(callablesForwardingRefererence_composed_referencesArePreserved(makeRawPipeFn));
}

// feature: data - move only
TEST(MakeRawPipe, callablesWithNonCopyableArguments_composed_works) {
  ASSERT_NO_FATAL_FAILURE(callablesWithNonCopyableArguments_composed_works(makeRawPipeFn));
}

}  // namespace funkypipes::test
