//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
#define FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP

#include <type_traits>
#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// This functor wraps a given function 'fn', allowing its argument and return value to be encapsulated in std::optional.
// If called with std::nullopt, the functor skips the execution of 'fn' and directly returns std::nullopt.
// Otherwise, it executes 'fn' and wraps the result in std::optional unless it is already an std::optional.
template <typename TFn>
class SkippableDecoratedFn {
 public:
  explicit SkippableDecoratedFn(TFn&& fn) : fn_(std::forward<TFn>(fn)) {}

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    using FnResult = std::invoke_result_t<TFn, TValue>;
    using OptionalFnResult = typename EnsureOptional<FnResult>::Type;

    if (optional_arg.has_value()) {
      return OptionalFnResult{fn_(std::move(optional_arg.value()))};
    }
    return OptionalFnResult{std::nullopt};
  }

 private:
  TFn fn_;
};

// Helper template function that transforms a given function into a skippable one. See SkippableDecoratedFn for details.
template <typename TFn>
auto makeSkippable(TFn&& fn) {
  return SkippableDecoratedFn<TFn>(std::forward<TFn>(fn));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
