//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_FUNKY_VOID_REMOVING_HPP
#define FUNKYPIPES_DETAILS_MAKE_FUNKY_VOID_REMOVING_HPP

#include <type_traits>
#include <utility>

#include "funkypipes/funky_void.hpp"

namespace funkypipes::details {

// Helper template function that decorates a given function by replacing a FunkyVoid return type by void.
// Other return types remain unchanged.
template <typename TFn>
auto makeFunkyVoidRemoving(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&& arg) mutable -> decltype(auto) {
    decltype(auto) result = fn_(std::forward<decltype(arg)>(arg));

    using ResultType = decltype(result);
    if constexpr (std::is_same_v<ResultType, FunkyVoid>) {
      // return void
      return;
    } else {
      // forward result as is
      if constexpr (std::is_rvalue_reference_v<decltype(result)>) {
        return std::move(result);
      } else {
        return result;
      }
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_FUNKY_VOID_REMOVING_HPP
