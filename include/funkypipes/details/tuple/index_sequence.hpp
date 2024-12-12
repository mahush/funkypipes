//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP

#include <array>
#include <tuple>
#include <utility>

namespace funkypipes::details {

//
// indexSequenceCat
//

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

//
// shiftIndexSequence
//

// A function that adds an offset to each value in an index_sequence, effectively shifting the sequence.
template <std::size_t Offset, std::size_t... Idxs>
constexpr auto shiftIndexSequence(std::index_sequence<Idxs...>) {
  return std::index_sequence<(Idxs + Offset)...>{};
}

//
// IndexSequenceSpan
//

// A template that represents a std::index_sequence containing the index values of the specified span.
template <std::size_t StartIdx, std::size_t Count>
using IndexSequenceSpan = decltype(shiftIndexSequence<StartIdx>(std::make_index_sequence<Count>{}));

//
// ComplementIndexSequence
//

template <std::size_t OverallIdxCount, std::size_t... IndicesToBeComplemented>
struct ComplementIndexSequenceImpl {
  // Check if index I is excluded
  template <std::size_t I>
  static constexpr bool is_excluded = ((I == IndicesToBeComplemented) || ...);

  // Generate a tuple with I if not excluded
  template <std::size_t I>
  using index_if_included = std::conditional_t<is_excluded<I>, std::index_sequence<>, std::index_sequence<I> >;

  // Convert indices to index_sequence
  template <std::size_t... Is>
  static constexpr auto make_indices(std::index_sequence<Is...>) {
    return indexSequenceCat(index_if_included<Is>{}...);
  }

  using type = decltype(make_indices(std::make_index_sequence<OverallIdxCount>{}));
};
template <std::size_t OverallIndexCount, std::size_t... IndicesToBeComplemented>
using ComplementIndexSequence =
    typename ComplementIndexSequenceImpl<OverallIndexCount, IndicesToBeComplemented...>::type;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
