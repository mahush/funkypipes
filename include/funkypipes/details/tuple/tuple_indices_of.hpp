//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_TUPLE_INDICES_OF_HPP
#define FUNKYPIPES_DETAILS_TUPLE_TUPLE_INDICES_OF_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/index_sequence.hpp"

namespace funkypipes::details {

namespace impl {

// Helper template that resolves the requested type to matching indices of the given tuple. The indices are provided as
// index sequence.
template <typename TRequested, typename TTuple>
struct TupleIndicesOfImpl;

template <typename TRequestedElement, typename... TElements>
struct TupleIndicesOfImpl<TRequestedElement, std::tuple<TElements...>> {
 private:
  template <std::size_t... Idxs>
  static auto findMatchingIndices(std::index_sequence<Idxs...>) {
    if constexpr (sizeof...(Idxs) == 0) {
      return std::index_sequence<>{};
    } else {
      return indexSequenceCat(std::conditional_t<std::is_same_v<TRequestedElement, std::decay_t<TElements>>,
                                                 std::index_sequence<Idxs>, std::index_sequence<>>{}...);
    }
  }

 public:
  using type = decltype(findMatchingIndices(std::index_sequence_for<TElements...>{}));
};

// Helper trait that ectends TupleIndicesOf by asserting that the requested element was found at least once.
template <typename TRequested, typename TTuple>
struct TupleIndicesOfAssertingSuccessImpl {
  using type = typename TupleIndicesOfImpl<TRequested, TTuple>::type;
  static_assert(type::size() >= 1, "At least one of the selected types is not available");
};

}  // namespace impl

// This template resolves the requested type to matching indices of the given tuple. The indices are provided as index
// sequence.
template <typename TRequested, typename TTuple>
using TupleIndicesOf = typename ::funkypipes::details::impl::TupleIndicesOfImpl<TRequested, TTuple>::type;

// like TupleIndicesOf but asserts that at least one matching index was found.
template <typename TRequested, typename TTuple>
using TupleIndicesOfAssertingSuccess =
    typename ::funkypipes::details::impl::TupleIndicesOfAssertingSuccessImpl<TRequested, TTuple>::type;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_TUPLE_INDICES_OF_HPP
