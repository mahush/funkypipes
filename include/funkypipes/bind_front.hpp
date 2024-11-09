//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_BIND_FRONT_HPP
#define FUNKYPIPES_BIND_FRONT_HPP

#include <functional>
#include <utility>

namespace funkypipes {

// Decorates the given function by pre-binding specified arguments from left to right, resulting in a new function that
// requires only the remaining arguments. This is a backport of C++20's bind_front.
template <typename TFn, typename... TArgsToBeBound>
auto bindFront(TFn&& fn, TArgsToBeBound&&... argsToBeBound) {
  return [fn_ = std::forward<TFn>(fn), boundArgs_ = std::make_tuple(std::forward<TArgsToBeBound>(argsToBeBound)...)](
             auto&&... args) mutable -> decltype(auto)  // Note: lambda needs to be mutable to make boundArgs_ moveable
  {
    return std::apply(
        [&](auto&&... boundArgs) {
          return std::invoke(fn_, std::forward<decltype(boundArgs)>(boundArgs)...,
                             std::forward<decltype(args)>(args)...);
        },
        std::move(boundArgs_));
  };
}
}  // namespace funkypipes

#endif  // FUNKYPIPES_BIND_FRONT_HPP
