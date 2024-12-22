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

#include "funkypipes/details/tuple/separate_tuple_elements.hpp"

using funkypipes::details::separateTupleElements;

// Ensure that the first element can be separated
TEST(SeparateTupleElements, threeElementTuple_separatingElement0_Element0AndElements12Returned) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [separated, remaining] = separateTupleElements<0>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 1);
  EXPECT_EQ(std::get<0>(separated), std::get<0>(original));

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 2);
  EXPECT_EQ(std::get<0>(remaining), std::get<1>(original));
  EXPECT_EQ(std::get<1>(remaining), std::get<2>(original));
}

// Ensure that a middle element can be separated
TEST(SeparateTupleElements, threeElementTuple_separatingElement1_Element0AndElements02Returned) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [separated, remaining] = separateTupleElements<1>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 1);
  EXPECT_EQ(std::get<0>(separated), std::get<1>(original));

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 2);
  EXPECT_EQ(std::get<0>(remaining), std::get<0>(original));
  EXPECT_EQ(std::get<1>(remaining), std::get<2>(original));
}

// Ensure that the last element can be separated
TEST(SeparateTupleElements, threeElementTuple_separatingElement2_Element0AndElements01Returned) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [separated, remaining] = separateTupleElements<2>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 1);
  EXPECT_EQ(std::get<0>(separated), std::get<2>(original));

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 2);
  EXPECT_EQ(std::get<0>(remaining), std::get<0>(original));
  EXPECT_EQ(std::get<1>(remaining), std::get<1>(original));
}

// Ensure that all available elements can be separated
TEST(SeparateTupleElements, oneElementTuple_separatingElement0_Element0AndEmptyTupleReturned) {
  // given
  auto original = std::make_tuple(1);

  // when
  auto [separated, remaining] = separateTupleElements<0>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 1);
  EXPECT_EQ(std::get<0>(separated), std::get<0>(original));

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 0);
}

// Ensure that multiple elements can be separated
TEST(SeparateTupleElements, threeElementTuple_separatingElement01_Element01AndElement2Returned) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [separated, remaining] = separateTupleElements<0, 1>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 2);
  EXPECT_EQ(std::get<0>(separated), std::get<0>(original));
  EXPECT_EQ(std::get<1>(separated), std::get<1>(original));

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 1);
  EXPECT_EQ(std::get<0>(remaining), std::get<2>(original));
}

// Ensure that separating zero elements works
TEST(SeparateTupleElements, threeElementTuple_separatingZeroElements_emptyTupleAndElements012Returned) {
  // given
  auto original = std::make_tuple(1, "two", '3');

  // when
  auto [separated, remaining] = separateTupleElements<>(original);

  // then
  EXPECT_EQ(std::tuple_size<decltype(separated)>(), 0);

  EXPECT_EQ(std::tuple_size<decltype(remaining)>(), 3);
  EXPECT_EQ(std::get<0>(remaining), std::get<0>(original));
  EXPECT_EQ(std::get<1>(remaining), std::get<1>(original));
  EXPECT_EQ(std::get<2>(remaining), std::get<2>(original));
}

// Ensure that const tuple is supported
TEST(SeparateTupleElements, constTuple_separated_works) {
  // given
  const std::string element0{"0"};
  const std::string element1{"1"};
  const auto original = std::make_tuple(element0, element1);

  // when
  auto [separated, remaining] = separateTupleElements<0>(original);

  // then
  EXPECT_EQ(std::get<0>(separated), "0");
  EXPECT_EQ(std::get<0>(remaining), "1");
}

// Ensure that rvalue elements are supported
TEST(SeparateTupleElements, tupleWithRValueElements_separated_elementsAccessableAsRValue) {
  // given
  struct NonCopyableStruct {
    explicit NonCopyableStruct(int value) : value_(value) {}
    ~NonCopyableStruct() = default;
    NonCopyableStruct(const NonCopyableStruct&) = delete;
    NonCopyableStruct(NonCopyableStruct&&) = default;
    NonCopyableStruct& operator=(const NonCopyableStruct&) = delete;
    NonCopyableStruct& operator=(NonCopyableStruct&&) = delete;

    int value_;
  };
  auto original = std::make_tuple(NonCopyableStruct{0}, NonCopyableStruct{1});

  // when
  auto [separated, remaining] = separateTupleElements<0>(std::move(original));

  // then
  EXPECT_EQ(std::get<0>(separated).value_, 0);
  auto element1 = std::get<0>(std::move(remaining));
  EXPECT_EQ(element1.value_, 1);
}

// TEST(SeparateTupleElements, TwoElementTuple_separatingThirdElement_TriggersStaticAssert) {
//   auto original = std::make_tuple(1, "two");
//
//   auto [separated, remaining] = separateTupleElements<2>(original);
// }

