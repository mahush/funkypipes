//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP
#define FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/tuple_traits.hpp"

namespace funkypipes::details {

// This function returns void for empty tuples, the only element for single-element tuples, and the tuple itself
// otherwise. It preserves references when returning a single element, and tuples are returned by value.
template <typename TTuple>
constexpr decltype(auto) tryFlattenTuple(TTuple&& tuple) {
  static_assert(IsTuple<std::decay_t<TTuple>>, "A tuple argument is required");
  constexpr auto size = std::tuple_size_v<std::decay_t<TTuple>>;
  if constexpr (size == 0) {
    return;
  } else if constexpr (size == 1) {
    // Note: Preserve element references
    using ElementType = std::tuple_element_t<0, std::decay_t<TTuple>>;
    if constexpr (std::is_reference_v<ElementType>) {
      return std::get<0>(std::forward<TTuple>(tuple));
    } else {
      return ElementType{std::get<0>(std::forward<TTuple>(tuple))};  // Note: return non references elements by value
    }
  } else {
    // Note: Always return tuples by value
    return std::decay_t<TTuple>{std::forward<TTuple>(tuple)};
  }
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_TRY_FLATTEN_TUPLE_HPP
