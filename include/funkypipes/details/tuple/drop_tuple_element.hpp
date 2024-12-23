//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP
#define FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP

#include <cstddef>
#include <tuple>
#include <utility>

#include "funkypipes/details/tuple/index_sequence.hpp"

namespace funkypipes::details {

namespace impl {

// Helper function that creates a new tuple based on the given one, using the specified indices. Only the
// elements at Idxs are accessed, all other elements are not touched.
template <typename TTuple, std::size_t... Idxs>
auto recreateTupleFromIndices(TTuple&& tuple, std::index_sequence<Idxs...>) {
  return std::make_tuple(std::get<Idxs>(std::forward<TTuple>(tuple))...);
}

// Helper function to determine the index sequences for elements before and after the element to remove and then invokes
// the tuple re-creation function.
template <std::size_t... IdxsToRemove, typename TTuple>
auto dropTupleElements(TTuple&& tuple) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;

  constexpr auto indicesToKeep = ComplementIndexSequence<tupleSize, IdxsToRemove...>{};
  return recreateTupleFromIndices(std::forward<TTuple>(tuple), indicesToKeep);
}

}  // namespace impl

// This function takes a lvalue tuple as input and returns a new tuple with the elements at IdxsToRemove removed. The
// elements at IdxsToRemove is not accessed.
template <std::size_t... IdxsToRemove, typename... TElements>
auto dropTupleElements(std::tuple<TElements...>& tuple) {
  return impl::dropTupleElements<IdxsToRemove...>(tuple);
}

// This function takes a const lvalue tuple as input and returns a new tuple with the elements at IdxsToRemove removed.
// The elements at IdxsToRemove is not accessed.
template <std::size_t... IdxsToRemove, typename... TElements>
auto dropTupleElements(const std::tuple<TElements...>& tuple) {
  return impl::dropTupleElements<IdxsToRemove...>(tuple);
}

// This function takes a rvalue tuple as input and returns a new tuple with the elements at IdxsToRemove removed. The
// elements at IdxsToRemove is not accessed.
template <std::size_t... IdxsToRemove, typename... TElements>
auto dropTupleElements(std::tuple<TElements...>&& tuple) {
  return impl::dropTupleElements<IdxsToRemove...>(std::move(tuple));
}

template <std::size_t... IdxsToRemove, typename... TElements>
auto dropTupleElements(std::index_sequence<IdxsToRemove...>, std::tuple<TElements...>& tuple) {
  return impl::dropTupleElements<IdxsToRemove...>(tuple);
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_DROP_TUPLE_ELEMENT_HPP
