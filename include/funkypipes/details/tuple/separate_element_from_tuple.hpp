//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_SEPARATE_ELEMENT_FROM_TUPLE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_SEPARATE_ELEMENT_FROM_TUPLE_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/tuple/drop_tuple_element.hpp"

namespace funkypipes::details {

namespace impl {

template <std::size_t ElementIdx, typename TTuple>
auto separateElementFromTuple(TTuple&& tuple) {
  constexpr std::size_t tupleSize = std::tuple_size<std::decay_t<TTuple>>::value;
  static_assert(ElementIdx < tupleSize, "Index out of range");

  // Note: Moving the tuple twice is intended and not an issue, as the element that was potentially moved
  // from is not accessed by the dropTupleElement function.
  return std::make_pair(std::get<ElementIdx>(std::forward<TTuple>(tuple)),
                        dropTupleElement<ElementIdx>(std::forward<TTuple>(tuple)));
}

}  // namespace impl

// This function takes an lvalue tuple as input and returns the separated element, along with a new tuple containing the
// remaining elements.
template <std::size_t ElementIndex, typename... TElements>
auto separateElementFromTuple(std::tuple<TElements...>& tuple) {
  return impl::separateElementFromTuple<ElementIndex>(tuple);
}

// This function takes an const lvalue tuple as input and returns the separated element, along with a new tuple
// containing the remaining elements.
template <std::size_t ElementIndex, typename... TElements>
auto separateElementFromTuple(const std::tuple<TElements...>& tuple) {
  return impl::separateElementFromTuple<ElementIndex>(tuple);
}

// This function takes an rvalue tuple as input and returns the separated element, along with a new tuple containing the
// remaining elements.
template <std::size_t ElementIndex, typename... TElements>
auto separateElementFromTuple(std::tuple<TElements...>&& tuple) {
  return impl::separateElementFromTuple<ElementIndex>(std::move(tuple));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_SEPARATE_ELEMENT_FROM_TUPLE_HPP
