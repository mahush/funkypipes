//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#ifndef FUNKYPIPES_ZIP_HPP
#define FUNKYPIPES_ZIP_HPP

#include <optional>
#include <type_traits>

namespace funkypipes {

// A function decorator that takes a function `fn` and optional arguments `optional_args`. If all `optional_args`
// contain a value, it calls `fn` with the values of these arguments and returns the result wrapped in an
// `std::optional`. If any argument does not have a value, it returns `std::nullopt`.
template <typename TFn, typename... TArgs>
auto zip(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](std::optional<TArgs>... optional_args) {
    using FnResultType = std::invoke_result_t<TFn, TArgs...>;
    using ZipResultType = std::optional<std::decay_t<FnResultType>>;

    if ((... && optional_args.has_value())) {
      return ZipResultType{fn_(optional_args.value()...)};
    } else {
      return ZipResultType{std::nullopt};
    }
  };
}
}  // namespace funkypipes

#endif  // FUNKYPIPES_ZIP_HPP
