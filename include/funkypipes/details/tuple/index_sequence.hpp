//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP

#include <tuple>
#include <utility>

namespace funkypipes::details {

// A function that concatenates two index sequences
template <std::size_t... IdxsA, std::size_t... IdxsB>
constexpr auto indexSequenceCat(std::index_sequence<IdxsA...>, std::index_sequence<IdxsB...>) {
  return std::index_sequence<IdxsA..., IdxsB...>{};
}

// A function that adds an offset to each value in an index_sequence, effectively shifting the sequence.
template <std::size_t Offset, std::size_t... Idxs>
constexpr auto shiftIndexSequence(std::index_sequence<Idxs...>) {
  return std::index_sequence<(Idxs + Offset)...>{};
}

// A template that represents a std::index_sequence containing the index values of the specified span.
template <std::size_t StartIdx, std::size_t Count>
using IndexSequenceSpan = decltype(shiftIndexSequence<StartIdx>(std::make_index_sequence<Count>{}));

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP