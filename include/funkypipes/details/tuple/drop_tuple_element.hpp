//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP
#define FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/tuple/index_sequence.hpp"

namespace funkypipes::details {

namespace impl {

// Helper function that creates a new tuple based on the given one, using the specified indices.
template <typename TTuple, std::size_t... Idxs>
auto recreateTupleFromIndices(TTuple&& tuple, std::index_sequence<Idxs...>) {
  return std::make_tuple(std::get<Idxs>(std::forward<TTuple>(tuple))...);
}

// Helper function to determine the index sequences for elements before and after the element to remove and then invokes
// the tuple re-creation function.
template <std::size_t IdxToRemove, typename TTuple>
auto dropTupleElement(TTuple&& tuple) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;

  static_assert(IdxToRemove < tupleSize, "Index out of range");

  constexpr auto elementCountBefore = IdxToRemove;
  constexpr auto idxsBefore = IndexSequenceSpan<0, elementCountBefore>{};

  constexpr auto elementCountAfter = tupleSize - IdxToRemove - 1;
  constexpr auto idxsAfter = IndexSequenceSpan<IdxToRemove + 1, elementCountAfter>{};

  return recreateTupleFromIndices(std::forward<TTuple>(tuple), indexSequenceCat(idxsBefore, idxsAfter));
}

}  // namespace impl

// This function takes a lvalue tuple as input and returns a new tuple with the element at IndexToRemove removed. The
// element at IndexToRemove is not accessed.
template <std::size_t IdxToRemove, typename... TElements>
auto dropTupleElement(std::tuple<TElements...>& tuple) {
  return impl::dropTupleElement<IdxToRemove>(tuple);
}

// This function takes a const lvalue tuple as input and returns a new tuple with the element at IndexToRemove removed.
// The element at IndexToRemove is not accessed.
template <std::size_t IdxToRemove, typename... TElements>
auto dropTupleElement(const std::tuple<TElements...>& tuple) {
  return impl::dropTupleElement<IdxToRemove>(tuple);
}

// This function takes a rvalue tuple as input and returns a new tuple with the element at IndexToRemove removed. The
// element at IndexToRemove is not accessed.
template <std::size_t IdxToRemove, typename... TElements>
auto dropTupleElement(std::tuple<TElements...>&& tuple) {
  return impl::dropTupleElement<IdxToRemove>(std::move(tuple));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP
