//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP

#include <tuple>
#include <utility>

namespace funkypipes::details {

template <typename TTuple>
constexpr decltype(auto) tryFlattenTuple(TTuple&& tuple) {
  constexpr auto size = std::tuple_size_v<std::decay_t<TTuple>>;
  if constexpr (size == 0) {
    return;
  } else if constexpr (size == 1) {
    return std::get<0>(std::forward<TTuple>(tuple));
  } else {
    return std::forward<TTuple>(tuple);
  }
};

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP
