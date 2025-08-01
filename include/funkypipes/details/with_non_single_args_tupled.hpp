//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_WITH_NON_SINGLE_ARGS_TUPLED_HPP
#define FUNKYPIPES_DETAILS_WITH_NON_SINGLE_ARGS_TUPLED_HPP

#include <cstddef>
#include <tuple>
#include <utility>

namespace funkypipes::details {

// Helper template function that transforms a given function accepting a single argument into a one that can accept
// multiple arguments by packing and forwarding them as tuple. Zero arguments are forwarded as empty tuple.
template <typename TFn>
auto withNonSingleArgsTupled(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable -> decltype(auto) {
    constexpr size_t args_count = sizeof...(args);
    if constexpr (args_count == 1) {
      // Note: single arguments are forwarded directly
      return fn_(std::forward<decltype(args)>(args)...);
    } else {
      // Note: zero or multiple arguments are forwarded via tuple, while preserving references
      return fn_(std::forward_as_tuple(std::forward<decltype(args)>(args)...));
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_WITH_NON_SINGLE_ARGS_TUPLED_HPP
