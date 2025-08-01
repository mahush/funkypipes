//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_WITH_VOID_RESULT_AS_EMPTY_TUPLE_HPP
#define FUNKYPIPES_DETAILS_WITH_VOID_RESULT_AS_EMPTY_TUPLE_HPP

#include <type_traits>
#include <utility>

#include "funkypipes/funky_void.hpp"

namespace funkypipes::details {

// Helper template function that decorates a given function by replacing a void return type with an empty tuple (aka
// FunkyVoid). Other return types remain unchanged.
template <typename TFn>
auto withVoidResultAsEmptyTuple(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable -> decltype(auto) {
    using ResultType = typename std::invoke_result<TFn, decltype(args)...>::type;
    if constexpr (std::is_void_v<ResultType>) {
      fn_(std::forward<decltype(args)>(args)...);
      return FunkyVoid{};
    } else {
      return fn_(std::forward<decltype(args)>(args)...);
    }
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_WITH_VOID_RESULT_AS_EMPTY_TUPLE_HPP
