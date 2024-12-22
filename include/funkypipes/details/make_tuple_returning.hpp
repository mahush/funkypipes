//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_TUPLE_RETURNING_HPP
#define FUNKYPIPES_DETAILS_MAKE_TUPLE_RETURNING_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/tuple/tuple_traits.hpp"

namespace funkypipes::details {

// Helper template function that decorates a given function by wrapping its return type with std::tuple.
template <typename TFn>
auto makeTupleReturning(TFn&& fn) {
  using funkypipes::details::IsTuple;

  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable -> decltype(auto) {
    using ResultType = typename std::invoke_result<TFn, decltype(args)...>::type;
    if constexpr (std::is_void_v<ResultType>) {
      fn_(std::forward<decltype(args)>(args)...);
      return std::tuple<>{};
    } else if constexpr (IsTuple<ResultType>) {
      return fn_(std::forward<decltype(args)>(args)...);
    } else {
      // Note: References are preserved
      if constexpr (std::is_reference_v<ResultType>) {
        return std::forward_as_tuple(fn_(std::forward<decltype(args)>(args)...));
      } else {
        return std::make_tuple(fn_(std::forward<decltype(args)>(args)...));
      }
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_TUPLE_RETURNING_HPP
