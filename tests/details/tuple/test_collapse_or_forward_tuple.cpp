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

#include "funkypipes/details/tuple/collapse_or_forward_tuple.hpp"
#include "utils/move_only_struct.hpp"

using funkypipes::details::collapseOrForwardTuple;

// Ensure that multiple elements tuple work
TEST(CollapseOrForwardTuple, tupleWithMultipleElements_tryFlattened_tupleReturnedUnchanged) {
  // given
  auto original = std::make_tuple(1, std::string{"two"});

  // when
  auto modified = collapseOrForwardTuple(original);

  // then
  static_assert(std::is_same_v<decltype(modified), std::tuple<int, std::string>>);
  EXPECT_EQ(modified, original);
}

// Ensure that single element tuple work
TEST(CollapseOrForwardTuple, tupleWithSingleElement_tryFlattened_elementReturned) {
  // given
  auto original = std::make_tuple(1);

  // when
  auto element = collapseOrForwardTuple(original);

  // then
  EXPECT_EQ(element, 1);
}

// Ensure that zero element tuple work
TEST(CollapseOrForwardTuple, tupleWithZeroElement_tryFlattened_voidReturned) {
  // given
  auto original = std::make_tuple();

  // when
  collapseOrForwardTuple(original);

  // then
  using ResultType = std::invoke_result_t<decltype(collapseOrForwardTuple<decltype(original)>), decltype(original)>;
  static_assert(std::is_same_v<ResultType, void>);
}

// Ensure that a lvalue reference tuple is returned by value
TEST(CollapseOrForwardTuple, lvalueTupleWithMultipleElements_tryFlattened_tupleReturnedByValue) {
  // given
  auto original = std::make_tuple(0, 1);

  // when
  decltype(auto) modified = collapseOrForwardTuple(original);

  // then
  static_assert(std::is_same_v<decltype(modified), std::tuple<int, int>>);
  EXPECT_EQ(std::get<0>(modified), 0);
  EXPECT_EQ(std::get<1>(modified), 1);
}

// Ensure that a const lvalue reference tuple is returned by value
TEST(CollapseOrForwardTuple, constLValueTupleWithMultipleElements_tryFlattened_tupleReturnedByValue) {
  // given
  auto original = std::make_tuple(0, 1);

  // when
  decltype(auto) modified = collapseOrForwardTuple(std::as_const(original));

  // then
  static_assert(std::is_same_v<decltype(modified), std::tuple<int, int>>);
  EXPECT_EQ(std::get<0>(modified), 0);
  EXPECT_EQ(std::get<1>(modified), 1);
}

// Ensure that a rvalue reference tuple is returned by value
TEST(CollapseOrForwardTuple, rvalueTupleWithMultipleElements_tryFlattened_tupleReturnedByValue) {
  // given
  auto original = std::make_tuple(0, 1);

  // when
  decltype(auto) modified = collapseOrForwardTuple(std::move(original));

  // then
  static_assert(std::is_same_v<decltype(modified), std::tuple<int, int>>);
  EXPECT_EQ(std::get<0>(modified), 0);
  EXPECT_EQ(std::get<1>(modified), 1);
}

// Ensure that a tuple with single value element leads to a value result
TEST(CollapseOrForwardTuple, lvalueTupleWithSingleElement_tryFlattened_elementReturnedByValue) {
  // given
  auto original = std::make_tuple(0);

  // when
  decltype(auto) outputElement = collapseOrForwardTuple(original);

  // then
  static_assert(std::is_same_v<decltype(outputElement), int>);
  EXPECT_EQ(outputElement, 0);
}

// Ensure that a tuple with single lvalue reference element leads to a lvalue reference result
TEST(CollapseOrForwardTuple, lvalueTupleWithSingleElement_tryFlattened_elementReturnedByLValueReference) {
  // given
  MoveOnlyStruct inputElement{0};
  auto original = std::forward_as_tuple(inputElement);

  // when
  decltype(auto) outputElement = collapseOrForwardTuple(original);

  // then
  static_assert(std::is_same_v<decltype(outputElement), MoveOnlyStruct&>);
  EXPECT_EQ(outputElement.value_, 0);
  outputElement.value_ = 1;
  EXPECT_EQ(inputElement.value_, 1);
}

// Ensure that a tuple with single const lvalue reference element leads to a const lvalue reference result
TEST(CollapseOrForwardTuple, constLValueTupleWithSingleElement_tryFlattened_elementReturnedByConstLValueReference) {
  // given
  MoveOnlyStruct inputElement{0};
  auto original = std::forward_as_tuple(std::as_const(inputElement));

  // when
  decltype(auto) outputElement = collapseOrForwardTuple(original);

  // then
  static_assert(std::is_same_v<decltype(outputElement), const MoveOnlyStruct&>);
  EXPECT_EQ(outputElement.value_, 0);
  inputElement.value_ = 1;
  EXPECT_EQ(outputElement.value_, 1);
}

// Ensure that a tuple with a single rvalue reference element leads to a rvalue reference result
TEST(CollapseOrForwardTuple, rvalueTupleWithRValueReferenceElement_tryFlattened_elementReturnedByRValueReference) {
  // given
  // NOLINTNEXTLINE misc-const-correctness: is about to be moved
  MoveOnlyStruct inputElement{0};  // Note: The object to move from needs still to be available
                                   // when accessing the rvalue reference in the end
  auto original = std::forward_as_tuple(std::move(inputElement));

  // when
  decltype(auto) outputElement = collapseOrForwardTuple(std::move(original));

  // then
  static_assert(std::is_same_v<decltype(outputElement), MoveOnlyStruct&&>);
  EXPECT_EQ(outputElement.value_, 0);
}

// Ensure that a tuple with a single move only value element leads to returning the element by value
TEST(CollapseOrForwardTuple, rvalueTupleWithMoveOnlyElement_tryFlattened_moveOnlyElementReturned) {
  // given
  auto original = std::make_tuple(MoveOnlyStruct{0});

  // when
  decltype(auto) outputElement = collapseOrForwardTuple(std::move(original));

  // then
  static_assert(std::is_same_v<decltype(outputElement), MoveOnlyStruct>);
  EXPECT_EQ(outputElement.value_, 0);
}
