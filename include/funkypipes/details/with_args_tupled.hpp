//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_WITH_ARGS_TUPLED_HPP
#define FUNKYPIPES_DETAILS_WITH_ARGS_TUPLED_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/tuple_traits.hpp"

namespace funkypipes::details {

// Helper template function that decorates a given function by forwarding any number of arguments packed in a tuple
// unless a tuple is already provided. References are preserved.
template <typename TFn>
auto withArgsTupled(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable -> decltype(auto) {
    constexpr size_t args_count = sizeof...(args);
    if constexpr (args_count == 1) {
      if constexpr (IsTuple<std::decay_t<decltype(args)>...>) {
        // Note: a tuple argument is forwarded as is
        return fn_(std::forward<decltype(args)>(args)...);
      } else {
        // Note: other arguments are forwarded via tuple, while preserving references
        return fn_(std::forward_as_tuple(std::forward<decltype(args)>(args)...));
      }
    } else {
      // Note: other arguments are forwarded via tuple, while preserving references
      return fn_(std::forward_as_tuple(std::forward<decltype(args)>(args)...));
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_WITH_ARGS_TUPLED_HPP
