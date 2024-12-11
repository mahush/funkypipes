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
#include <cstddef>
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

template <std::size_t Count, std::size_t... Idxs>
constexpr auto makeComplement(std::index_sequence<Idxs...>) {}

// template <typename TArray, std::size_t... Is>
// constexpr auto arrayToIndexSequenceImpl(const TArray& array, std::index_sequence<Is...>) {
//   return std::index_sequence<array[Is]...>{};
// }
//
// template <std::size_t ArraySize>
// constexpr auto arrayToIndexSequence(const std::array<std::size_t, ArraySize>& array) {
//   return arrayToIndexSequenceImpl(array, std::index_sequence<ArraySize>{});
// }

template <std::size_t N, std::size_t... IndicesToRemove>
struct ComplementIndicesImpl {
  static constexpr std::size_t NumComplement = N - sizeof...(IndicesToRemove);

  template <std::size_t... Is>
  static constexpr auto make_index_sequence_impl(std::index_sequence<Is...>) {
    constexpr auto complement_indices_array = [] {
      constexpr auto removed = [] {
        std::array<bool, N> arr{};
        ((arr[IndicesToRemove] = true), ...);
        return arr;
      }();

      std::array<std::size_t, NumComplement> arr{};
      std::size_t idx = 0;
      for (std::size_t i = 0; i < N; ++i) {
        if (!removed[i]) {
          arr[idx++] = i;
        }
      }
      return arr;
    }();

    return std::index_sequence<complement_indices_array[Is]...>{};
    //    return arrayToIndexSequence(complement_indices_array);
  }

  using type = decltype(make_index_sequence_impl(std::make_index_sequence<NumComplement>{}));
};

template <std::size_t N, std::size_t... IndicesToRemove>
using ComplementIndices = typename ComplementIndicesImpl<N, IndicesToRemove...>::type;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_INDEX_SEQUENCE_HPP
