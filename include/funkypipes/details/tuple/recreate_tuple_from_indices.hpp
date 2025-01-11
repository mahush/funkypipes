//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_RECREATE_TUPLE_FROM_INDICES_HPP
#define FUNKYPIPES_DETAILS_TUPLE_RECREATE_TUPLE_FROM_INDICES_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/index_sequence.hpp"

namespace funkypipes::details {

// Functions that creates a new tuple based on the given one, using the specified indices. Only the
// elements at Idxs are accessed, all other elements are not touched. The original element types are preserved.
template <std::size_t... Idxs, typename TTuple>
auto recreateTupleFromIndices(TTuple&& tuple, std::index_sequence<Idxs...>) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;
  static_assert(((Idxs < tupleSize) && ...), "Index out of range");

  using ResultTuple = std::tuple<std::tuple_element_t<Idxs, std::decay_t<TTuple>>...>;
  return ResultTuple{std::get<Idxs>(std::forward<TTuple>(tuple))...};
}

template <std::size_t... Idxs, typename... TElements>
auto recreateTupleFromIndices(const std::tuple<TElements...>& tuple) {
  return recreateTupleFromIndices(tuple, std::index_sequence<Idxs...>{});
}

template <std::size_t... Idxs, typename... TElements>
auto recreateTupleFromIndices(std::tuple<TElements...>&& tuple) {
  return recreateTupleFromIndices(std::move(tuple), std::index_sequence<Idxs...>{});
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_RECREATE_TUPLE_FROM_INDICES_HPP
