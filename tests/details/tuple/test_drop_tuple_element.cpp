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

#include "funkypipes/details/tuple/drop_tuple_element.hpp"

using namespace funkypipes::details;

TEST(DropTupleElement, threeElementTuple_removingFirstElement_lastTwoElementsLeft) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto modified = dropTupleElement<0>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(modified)>(), std::tuple_size<decltype(original)>::value - 1);
  EXPECT_EQ(std::get<0>(modified), std::get<1>(original));
  EXPECT_EQ(std::get<1>(modified), std::get<2>(original));
}

TEST(DropTupleElement, threeElementTuple_removingIntermediateElement_firstAndLastElementLeft) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto modified = dropTupleElement<1>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(modified)>(), std::tuple_size<decltype(original)>::value - 1);
  EXPECT_EQ(std::get<0>(modified), std::get<0>(original));
  EXPECT_EQ(std::get<1>(modified), std::get<2>(original));
}

TEST(DropTupleElement, threeElementTuple_removingLastElement_firstTwoElementsLeft) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto modified = dropTupleElement<2>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(modified)>(), std::tuple_size<decltype(original)>::value - 1);
  EXPECT_EQ(std::get<0>(modified), std::get<0>(original));
  EXPECT_EQ(std::get<1>(modified), std::get<1>(original));
}

TEST(DropTupleElement, constLValueTuple_removeCalled_works) {
  // given
  const auto original = std::make_tuple(0, 1);

  // when
  auto modified = dropTupleElement<0>(original);

  // then
  EXPECT_EQ(std::get<0>(std::move(modified)), 1);
}

TEST(DropTupleElement, tupleWithRValueElements_oneElementRemoved_otherElementAccessableAsRValue) {
  // given
  struct NonCopyableStruct {
    explicit NonCopyableStruct(int value) : m_value(value) {}
    ~NonCopyableStruct() = default;
    NonCopyableStruct(const NonCopyableStruct&) = delete;
    NonCopyableStruct(NonCopyableStruct&&) = default;
    NonCopyableStruct& operator=(const NonCopyableStruct&) = delete;
    NonCopyableStruct& operator=(NonCopyableStruct&&) = delete;

    int m_value;
  };
  auto original = std::make_tuple(NonCopyableStruct{0}, NonCopyableStruct{1});

  // when
  auto modified = dropTupleElement<0>(std::move(original));

  // then
  auto element = std::get<0>(std::move(modified));
  EXPECT_EQ(element.m_value, 1);
}

// TEST(DropTupleElement, twoElementTuple_removingThirdElement_triggersStaticAssert) {
//   auto original = std::make_tuple(1, "two");
//
//   auto modified = removeTupleElement<2>(original);
// }

