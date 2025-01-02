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

#include "funkypipes/details/tuple/resolve_rvalue_references.hpp"

using funkypipes::details::resolveRValueReferences;

// Ensure that lvalue tuples are supported
TEST(ResolveRValueReferences, lvalueTuple_resolveRValueReferencesCalled_validTupleReturned) {
  // given
  auto tuple = std::make_tuple(1);

  // when
  auto result = resolveRValueReferences(tuple);

  // then
  ASSERT_EQ(result, std::make_tuple(1));
}

// Ensure that const lvalue tuples are supported
TEST(ResolveRValueReferences, constLValueTuple_resolveRValueReferencesCalled_validTupleReturned) {
  // given
  const auto tuple = std::make_tuple(2);

  // when
  auto result = resolveRValueReferences(tuple);

  // then
  ASSERT_EQ(result, std::make_tuple(2));
}

// Ensure that rvalue tuples are supported
TEST(ResolveRValueReferences, rvalueTuple_resolveRValueReferencesCalled_validTupleReturned) {
  // given
  auto tuple = std::make_tuple(3);

  // when
  auto result = resolveRValueReferences(std::move(tuple));

  // then
  ASSERT_EQ(result, std::make_tuple(3));
}

// Ensure that rvalue references are resolved
TEST(ResolveRValueReferences, tupleOfRValueReferences_resolveRValueReferencesCalled_tupleContainsValueElements) {
  // given
  auto arg1{4};
  std::string arg2{"five"};
  auto tuple = std::forward_as_tuple(std::move(arg1), std::move(arg2));

  // when
  auto result = resolveRValueReferences(tuple);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int, std::string>>);
  ASSERT_EQ(result, std::make_tuple(4, std::string{"five"}));
}

// Ensure that lvalue references are preserved
TEST(ResolveRValueReferences, tupleOfLValueReference_resolveRValueReferencesCalled_tupleStillContainsLValueReference) {
  // given
  int arg{6};
  auto tuple = std::forward_as_tuple(arg);

  // when
  auto result = resolveRValueReferences(tuple);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&>>);
  ASSERT_EQ(result, std::make_tuple(6));
}

// Ensure that const lvalue references are preserved
TEST(ResolveRValueReferences,
     tupleOfConstLValueReference_resolveRValueReferencesCalled_tupleStillContainsConstLValueReference) {
  // given
  int arg{7};
  auto tuple = std::forward_as_tuple(std::as_const(arg));

  // when
  auto result = resolveRValueReferences(tuple);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&>>);
  ASSERT_EQ(result, std::make_tuple(7));
  arg++;
  ASSERT_EQ(result, std::make_tuple(8));
}

