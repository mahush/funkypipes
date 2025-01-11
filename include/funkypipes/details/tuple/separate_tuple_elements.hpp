//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_SEPARATE_TUPLE_ELEMENTS_HPP
#define FUNKYPIPES_DETAILS_TUPLE_SEPARATE_TUPLE_ELEMENTS_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/index_sequence.hpp"
#include "funkypipes/details/tuple/recreate_tuple_from_indices.hpp"

namespace funkypipes::details {

namespace impl {

// Implementation of separating the specified elements of a given tuple into two tuples, one that contains the separated
// elements and another containing the remaining ones.
template <std::size_t... IdxsToSeparate, typename TTuple>
auto separateTupleElements(TTuple&& tuple) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;
  static_assert(((IdxsToSeparate < tupleSize) && ...), "Index out of range");

  // Note: Forwarding the tuple twice is intended and not an issue, as recreateTupleFromIndices does only modify the
  // specified tuple elements which are distinct for both calls

  auto separatedIndices = std::index_sequence<IdxsToSeparate...>{};
  auto separatedElements = recreateTupleFromIndices(std::forward<TTuple>(tuple), std::move(separatedIndices));

  auto remainingIndices = ComplementIndexSequence<tupleSize, IdxsToSeparate...>{};
  auto remainingElements = recreateTupleFromIndices(std::forward<TTuple>(tuple), std::move(remainingIndices));

  return std::make_pair(std::move(separatedElements), std::move(remainingElements));
}

}  // namespace impl

// This function takes an const lvalue tuple as input and returns the separated element, along with a new tuple
// containing the remaining elements.
template <std::size_t... IdxsToSeparate, typename... TElements>
auto separateTupleElements(const std::tuple<TElements...>& tuple) {
  return impl::separateTupleElements<IdxsToSeparate...>(tuple);
}

// These functions takes an rvalue tuple as input and returns the separated element, along with a new tuple containing
// the remaining elements.
template <std::size_t... IdxsToSeparate, typename... TElements>
auto separateTupleElements(std::tuple<TElements...>&& tuple) {
  return impl::separateTupleElements<IdxsToSeparate...>(std::move(tuple));
}
template <std::size_t... IdxsToSeparate, typename... TElements>
auto separateTupleElements(std::tuple<TElements...>&& tuple, const std::index_sequence<IdxsToSeparate...>&) {
  return impl::separateTupleElements<IdxsToSeparate...>(std::move(tuple));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_SEPARATE_TUPLE_ELEMENTS_HPP
