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

//
// ComplementIndexSequence
//

namespace impl {
template <std::size_t SequenceSize, std::size_t... IdxsToBeComplemented>
struct ComplementIndexSequenceImpl {
  template <std::size_t Idx>
  static constexpr bool is_complement = ((Idx != IdxsToBeComplemented) && ...);

  template <std::size_t Idx>
  using index_if_complement = std::conditional_t<is_complement<Idx>, std::index_sequence<Idx>, std::index_sequence<> >;

  template <std::size_t... Idxs>
  static constexpr auto makeComplementIndexSequence(std::index_sequence<Idxs...>) {
    static_assert(SequenceSize >= sizeof...(IdxsToBeComplemented), "Sequence size smaller then given indices");
    if constexpr (SequenceSize == 0) {
      return std::index_sequence<>{};
    } else {
      return indexSequenceCat(index_if_complement<Idxs>{}...);
    }
  }

  using type = decltype(makeComplementIndexSequence(std::make_index_sequence<SequenceSize>{}));
};
}  // namespace impl

// A template to generates a complementary index sequence for the given size and indices
template <std::size_t OverallIndexCount, std::size_t... IndicesToBeComplemented>
using ComplementIndexSequence =
    typename impl::ComplementIndexSequenceImpl<OverallIndexCount, IndicesToBeComplemented...>::type;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
