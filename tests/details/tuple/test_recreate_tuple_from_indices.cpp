//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/recreate_tuple_from_indices.hpp"

using funkypipes::details::recreateTupleFromIndices;
using namespace std::string_literals;

// Ensure that recreation for a single index works
TEST(RecreateTupleFromIndices, tupleWithTwoElements_recreatedAt0_returnsTupleWithElement0) {
  // given
  auto original = std::make_tuple(1, "two"s);

  // when
  auto result = recreateTupleFromIndices<0>(original);

  // then
  EXPECT_EQ(result, std::make_tuple(1));
}

// Ensure that recreation for multiple indices works
TEST(RecreateTupleFromIndices, tupleWithTwoElements_recreatedAt01_returnsTupleWithElements0and1) {
  // given
  auto original = std::make_tuple(1, "two"s);

  // when
  auto result = recreateTupleFromIndices<0, 1>(original);

  // then
  EXPECT_EQ(result, std::make_tuple(1, "two"s));
}

// Ensure that the index order is free to choose
TEST(RecreateTupleFromIndices, tupleWithTwoElements_recreatedInReverse_returnsReversedTuple) {
  // given
  auto original = std::make_tuple(1, "two"s);

  // when
  auto result = recreateTupleFromIndices<1, 0>(original);

  // then
  EXPECT_EQ(result, std::make_tuple("two"s, 1));
}

// Ensure that const lvlaue references are preserved
TEST(RecreateTupleFromIndices, tupleWithConstLValueReferences_recreated_referencesArePreserved) {
  // given
  auto arg1 = 1;
  auto arg2 = "two"s;
  auto original = std::forward_as_tuple(std::as_const(arg1), std::as_const(arg2));

  // when
  auto result = recreateTupleFromIndices<0, 1>(original);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<const int&, const std::string&>>);
  EXPECT_EQ(result, std::make_tuple(1, "two"s));
  arg1 = 2;  // NOLINT clang-analyzer-deadcode.DeadStores: false positive
  arg2 = "three";
  EXPECT_EQ(result, std::make_tuple(2, "three"));
}

// Ensure that lvlaue references are preserved
TEST(RecreateTupleFromIndices, tupleWithLValueReferences_recreated_referencesArePreserved) {
  // given
  auto arg1 = 1;
  auto arg2 = "two"s;
  auto original = std::forward_as_tuple(arg1, arg2);

  // when
  auto result = recreateTupleFromIndices<0, 1>(original);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&, std::string&>>);
  EXPECT_EQ(result, std::make_tuple(1, "two"s));
  std::get<0>(result) = 2;
  std::get<1>(result) = "three";
  EXPECT_EQ(arg1, 2);
  EXPECT_EQ(arg2, "three");
}

// Ensure that rvlaue references are preserved
TEST(RecreateTupleFromIndices, tupleWithRValueReferences_recreated_referencesArePreserved) {
  // given
  auto arg1 = 1;
  auto arg2 = "two"s;
  auto original = std::forward_as_tuple(std::move(arg1), std::move(arg2));

  // when
  auto result = recreateTupleFromIndices<0, 1>(std::move(original));

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int&&, std::string&&>>);
  EXPECT_EQ(result, std::make_tuple(1, "two"s));
}

// Ensure that tuple is returned by value
TEST(RecreateTupleFromIndices, tuple_recreated_returnsByValue) {
  // given
  auto original = std::make_tuple(1);

  // when
  decltype(auto) result = recreateTupleFromIndices<0>(original);

  // then
  static_assert(std::is_same_v<decltype(result), std::tuple<int>>);
}

// Ensure that out of bound indices trigger a static_assert
// TEST(RecreateTupleFromIndices, tupleWithTwoElements_outOfBoundIndex_triggersStaticAssert) {
//  // given
//  auto original = std::make_tuple(1, "two"s);
//
//  // when/then
//  auto result = recreateTupleFromIndices<0, 2>(original);
//}

