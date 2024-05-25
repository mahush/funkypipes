//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
#define FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP

#include <optional>
#include <type_traits>
#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// This functor wraps a given function 'fn', guaranteeing its return value to be encapsulated in std::optional.
// If called with std::nullopt, the functor skips the execution of 'fn' and directly returns std::nullopt.
// Otherwise, it executes 'fn' and wraps the result in std::optional unless it is already an std::optional.
template <typename TFn>
class SkippableFn {
 public:
  explicit SkippableFn(TFn&& fn) : fn_(std::forward<TFn>(fn)) {}

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    return callOrSkip<TValue>(std::move(optional_arg));
  }

  template <typename TValue>
  inline auto operator()(std::optional<TValue>& optional_arg) {
    return callOrSkip<TValue>(optional_arg);
  }

 private:
  // Please note: In case fn result is a referene to an optional it is not possible to just forward it here because
  // in case of skipping a std::nullopt rvalue needs to be returned.
  template <typename TValue, typename TOptional>
  inline auto callOrSkip(TOptional&& optional_arg) {
    using FnArgumentType = typename OptionalUnwrapping<std::optional<TValue>>::Type;
    using RawFnResult = std::invoke_result_t<TFn, FnArgumentType>;

    if constexpr (IsOptional<RawFnResult>::value) {
      if (optional_arg.has_value()) {
        return fn_(std::forward<TOptional>(optional_arg).value());
      }
      return RawFnResult{std::nullopt};
    } else {
      using OptionalWrappedFnResult = typename OptionalWrapping<RawFnResult>::Type;
      if (optional_arg.has_value()) {
        return OptionalWrappedFnResult{fn_(std::forward<TOptional>(optional_arg).value())};
      }
      return OptionalWrappedFnResult{std::nullopt};
    }
  }

  TFn fn_;
};

// Helper template function that transforms a given function into a skippable one. See SkippableFn for details.
template <typename TFn>
auto makeSkippable(TFn&& fn) {
  return SkippableFn<TFn>(std::forward<TFn>(fn));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
