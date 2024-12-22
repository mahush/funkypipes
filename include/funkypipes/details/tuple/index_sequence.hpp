//
// Copyright (c) 2024 mahush (info@mahush.de)
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

// Operator concatanating two index sequences
namespace impl {
template <std::size_t... Idxs1, std::size_t... Idxs2>
constexpr auto operator+(std::index_sequence<Idxs1...>, std::index_sequence<Idxs2...>) {
  return std::index_sequence<Idxs1..., Idxs2...>{};
}
}  // namespace impl

// A function that concatenates any number of index_sequence objects
template <typename... TIndexSequences>
constexpr auto indexSequenceCat(TIndexSequences...) {
  using impl::operator+;
  return (TIndexSequences{} + ...);
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
