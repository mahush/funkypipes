//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP

#include <utility>

namespace funkypipes::details {
//
// indexSequenceCat
//

// Operator concatenating two index sequences
namespace impl {
template <std::size_t... LhsIdxs, std::size_t... RhsIdxs>
constexpr auto operator+(std::index_sequence<LhsIdxs...>, std::index_sequence<RhsIdxs...>) {
  return std::index_sequence<LhsIdxs..., RhsIdxs...>{};
}
}  // namespace impl

// A function that concatenates any number of index_sequence objects
template <typename... TIndexSequences>
constexpr auto indexSequenceCat(TIndexSequences...) {
  using ::funkypipes::details::impl::operator+;
  return (TIndexSequences{} + ...);
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
