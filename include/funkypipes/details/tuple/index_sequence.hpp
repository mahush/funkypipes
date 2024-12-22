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

//
// ComplementIndexSequence
//

namespace impl {

// Helper template that generates the complementary index sequence for the given size and indices
template <std::size_t SequenceSize, std::size_t... IdxsToBeComplemented>
struct ComplementIndexSequenceImpl {
  template <std::size_t Idx>
  static constexpr bool is_complement = ((Idx != IdxsToBeComplemented) && ...);

  template <std::size_t Idx>
  using index_if_complement = std::conditional_t<is_complement<Idx>, std::index_sequence<Idx>, std::index_sequence<> >;

  template <std::size_t... Idxs>
  static constexpr auto makeComplementIndexSequence(std::index_sequence<Idxs...>) {
    static_assert(SequenceSize >= sizeof...(IdxsToBeComplemented),
                  "The sequence size cannot be smaller than the number of indices to be complemented.");
    if constexpr (SequenceSize == 0) {
      return std::index_sequence<>{};
    } else {
      return indexSequenceCat(index_if_complement<Idxs>{}...);
    }
  }

  using type = decltype(makeComplementIndexSequence(std::make_index_sequence<SequenceSize>{}));
};
}  // namespace impl

// A template that generates a complementary index sequence for the given size and indices
template <std::size_t OverallIndexCount, std::size_t... IdxsToBeComplemented>
using ComplementIndexSequence =
    typename impl::ComplementIndexSequenceImpl<OverallIndexCount, IdxsToBeComplemented...>::type;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
