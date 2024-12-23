//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_WHEN_TYPE_HPP
#define FUNKYPIPES_WHEN_TYPE_HPP

#include <iostream>
#include <type_traits>

namespace funkypipes {

template <typename T, typename Func>
auto whenType(Func func) {
  return [func](auto&& arg) -> decltype(auto) {
    if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, T>) {
      return func(std::forward<decltype(arg)>(arg));
    } else {
      return std::forward<decltype(arg)>(arg);
    }
  };
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_WHEN_TYPE_HPP
