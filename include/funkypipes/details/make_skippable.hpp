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

  // Note: Non-virtual destructor is intentional to avoid vtable generation. Although this class also serves as a base
  // class, it is not intended to be used polymorphically.
  ~SkippableFn() = default;

  SkippableFn(const SkippableFn&) = default;
  SkippableFn(SkippableFn&&) noexcept = default;
  SkippableFn& operator=(const SkippableFn&) = default;
  SkippableFn& operator=(SkippableFn&&) noexcept = default;

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    return callOrSkipFn<TValue>(std::move(optional_arg));
  }

  template <typename TValue>
  inline auto operator()(std::optional<TValue>& optional_arg) {
    return callOrSkipFn<TValue>(optional_arg);
  }

  template <typename TValue>
  inline auto operator()(const std::optional<TValue>& optional_arg) {
    return callOrSkipFn<TValue>(optional_arg);
  }

 protected:
  template <typename TArg>
  inline decltype(auto) callFn(TArg&& arg) {
    return fn_(std::forward<TArg>(arg));
  }

  template <typename TValue, typename TOptional>
  inline auto callOrSkipFn(TOptional&& optional_arg) {
    // Please note: If the result of fn is a reference to an optional, it is not possible to simply forward the
    // reference here, because in the case of skipping, a new std::nullopt optional rvalue needs to be returned. Thus
    // the signature of this function always returns a copy.

    using RawFnResult = std::invoke_result_t<TFn, TValue>;

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

 private:
  TFn fn_;
};

// Helper template function that transforms a given function into a skippable one. See SkippableFn for details.
template <typename TFn>
auto makeSkippable(TFn&& fn) {
  return SkippableFn<TFn>(std::forward<TFn>(fn));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
