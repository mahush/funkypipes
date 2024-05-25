//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_TUPLE_PACKING_HPP
#define FUNKYPIPES_DETAILS_MAKE_TUPLE_PACKING_HPP

#include <cstddef>
#include <tuple>
#include <utility>

namespace funkypipes::details {

// Helper template function that transforms a given function accepting a single argument into a one that can accept
// multiple arguments by packing and forwarding them as tuple. Zero arguments are forwarded as empty tuple.
template <typename TFn>
auto makeTuplePacking(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable -> decltype(auto) {
    constexpr size_t args_count = sizeof...(args);
    if constexpr (args_count == 1) {
      // Note: single arguments are forwarded directly
      return fn_(std::forward<decltype(args)>(args)...);
    } else {
      // Note: zero or multiple arguments are forwarded via tuple
      return fn_(std::make_tuple(std::forward<decltype(args)>(args)...));
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_TUPLE_PACKING_HPP
