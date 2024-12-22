//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include <tuple>
#include <utility>

#include "funkypipes/details/tuple/try_flatten_tuple.hpp"

using funkypipes::details::tryFlattenTuple;

TEST(TryFlattenTuple, valueTupleWithMultipleElements_tryFlattened_valueReferenceReturned) {
  // given
  auto original = std::make_tuple(1, "two");

  // when
  auto modified = tryFlattenTuple(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(modified)>(), std::tuple_size<decltype(original)>::value - 1);
  EXPECT_EQ(std::get<0>(modified), std::get<0>(original));
  EXPECT_EQ(std::get<1>(modified), std::get<1>(original));
}

TEST(TryFlattenTuple, valueTupleWithSingleElement_tryFlattened_elementReturned) {
  // given
  auto original = std::make_tuple(1);

  // when
  auto element = tryFlattenTuple(original);

  // then
  EXPECT_EQ(element, 1);
}

TEST(TryFlattenTuple, valueTupleWithZeroElement_tryFlattened_voidReturned) {
  // given
  auto original = std::make_tuple();

  // when
  tryFlattenTuple(original);

  // then
  using ResultType = std::invoke_result_t<decltype(tryFlattenTuple<decltype(original)>), decltype(original)>;
  static_assert(std::is_same_v<ResultType, void>, "Result type is not void");
}

struct NonCopyableStruct {
  explicit NonCopyableStruct(int value) : m_value(value) {}
  ~NonCopyableStruct() = default;
  NonCopyableStruct(const NonCopyableStruct&) = delete;
  NonCopyableStruct(NonCopyableStruct&&) = default;
  NonCopyableStruct& operator=(const NonCopyableStruct&) = delete;
  NonCopyableStruct& operator=(NonCopyableStruct&&) = delete;

  int m_value;
};

TEST(TryFlattenTuple, lvalueTupleWithMultipleElements_tryFlattened_lvalueReferenceReturned) {
  // given
  auto original = std::make_tuple(NonCopyableStruct{0}, NonCopyableStruct{1});

  // when
  auto& modified = tryFlattenTuple(original);

  // then
  EXPECT_EQ(std::get<0>(std::move(modified)).m_value, 0);
  EXPECT_EQ(std::get<1>(std::move(modified)).m_value, 1);
}

TEST(TryFlattenTuple, constLValueTupleWithMultipleElements_tryFlattened_ConstLValueReferenceReturned) {
  // given
  const auto original = std::make_tuple(NonCopyableStruct{0}, NonCopyableStruct{1});

  // when
  const auto& modified = tryFlattenTuple(original);

  // then
  EXPECT_EQ(std::get<0>(std::move(modified)).m_value, 0);
  EXPECT_EQ(std::get<1>(std::move(modified)).m_value, 1);
}

TEST(TryFlattenTuple, lvalueTupleWithSingleElement_tryFlattened_lvalueReferenceReturned) {
  // given
  auto original = std::make_tuple(NonCopyableStruct{0});

  // when
  auto& element = tryFlattenTuple(original);

  // then
  EXPECT_EQ(element.m_value, 0);
}

TEST(TryFlattenTuple, constLValueTupleWithSingleElement_tryFlattened_constLValueReferenceReturned) {
  // given
  const auto original = std::make_tuple(NonCopyableStruct{0});

  // when
  const auto& element = tryFlattenTuple(original);

  // then
  EXPECT_EQ(element.m_value, 0);
}

// TEST(TryFlattenTuple, twoElementTuple_removingThirdElement_triggersStaticAssert) {
//   auto original = std::make_tuple(1, "two");
//
//   auto modified = removeTupleElement<2>(original);
// }

