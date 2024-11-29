//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_REMOVE_TUPLE_ELEMENT_HPP
#define FUNKYPIPES_DETAILS_REMOVE_TUPLE_ELEMENT_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/index_sequence.hpp"

namespace funkypipes::details {

// This helper function creates a new tuple based on the given one, using the specified indices.
template <typename TTuple, std::size_t... IdxsPart1, std::size_t... IdxsPart2>
auto recreateTupleFromIndices(TTuple&& tuple, std::index_sequence<IdxsPart1...>, std::index_sequence<IdxsPart2...>) {
  return std::make_tuple(std::get<IdxsPart1>(std::forward<TTuple>(tuple))...,
                         std::get<IdxsPart2>(std::forward<TTuple>(tuple))...);
}

// Helper function to determine the index sequences for elements before and after the element to remove and then invokes
// the tuple re-creation function.
template <std::size_t IdxToRemove, typename TTuple>
auto removeTupleElementImpl(TTuple&& tuple) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;

  static_assert(IdxToRemove < tupleSize, "Index out of range");

  constexpr auto elementCountBefore = IdxToRemove;
  constexpr auto idxsBefore = IndexSequenceSpan<0, elementCountBefore>{};

  constexpr auto elementCountAfter = tupleSize - IdxToRemove - 1;
  constexpr auto idxsAfter = IndexSequenceSpan<IdxToRemove + 1, elementCountAfter>{};

  return recreateTupleFromIndices(std::forward<TTuple>(tuple), idxsBefore, idxsAfter);
}

// This function takes a lvalue tuple as input and returns a new tuple with the element at IndexToRemove removed.
template <std::size_t IdxToRemove, typename... TElements>
auto removeTupleElement(std::tuple<TElements...>& tuple) {
  return removeTupleElementImpl<IdxToRemove>(tuple);
}

// This function takes a const lvalue tuple as input and returns a new tuple with the element at IndexToRemove removed.
template <std::size_t IdxToRemove, typename... TElements>
auto removeTupleElement(const std::tuple<TElements...>& tuple) {
  return removeTupleElementImpl<IdxToRemove>(tuple);
}

// This function takes a rvalue tuple as input and returns a new tuple with the element at IndexToRemove removed.
template <std::size_t IdxToRemove, typename... TElements>
auto removeTupleElement(std::tuple<TElements...>&& tuple) {
  return removeTupleElementImpl<IdxToRemove>(std::move(tuple));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_REMOVE_TUPLE_ELEMENT_HPP
