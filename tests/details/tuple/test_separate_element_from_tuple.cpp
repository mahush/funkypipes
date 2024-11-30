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

#include "funkypipes/details/tuple/separate_element_from_tuple.hpp"

using namespace funkypipes::details;

TEST(SeparateElementFromTuple, threeElementTuple_separatingFirstElement_lastTwoElementsLeft) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [element, modified] = separateElementFromTuple<0>(original);

  // then
  EXPECT_EQ(element, std::get<0>(original));

  EXPECT_EQ(std::tuple_size<decltype(modified)>(), std::tuple_size<decltype(original)>::value - 1);
  EXPECT_EQ(std::get<0>(modified), std::get<1>(original));
  EXPECT_EQ(std::get<1>(modified), std::get<2>(original));
}

TEST(SeparateElementFromTuple, tupleWithRValueElements_separated_elementsAccessableAsRValue) {
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
  auto [element0, modified] = separateElementFromTuple<0>(std::move(original));

  // then
  EXPECT_EQ(element0.m_value, 0);
  auto element1 = std::get<0>(std::move(modified));
  EXPECT_EQ(element1.m_value, 1);
}

// TEST(SeparateElementFromTuple, TwoElementTuple_separatingThirdElement_TriggersStaticAssert) {
//   auto original = std::make_tuple(1, "two");
//
//  auto [element, modified] = separateElementFromTuple<2>(original);
// }

