//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_FALLBACK_HPP
#define FUNKYPIPES_DETAILS_MAKE_FALLBACK_HPP

#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// Once this functor is called, it returnes the optional argument's value if present; otherwise the given function is
// called and their result returned.
template <typename TFn>
class FallbackFn {
 public:
  explicit FallbackFn(TFn&& fallback_fn) : fallback_fn_(std::forward<TFn>(fallback_fn)) {}

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    return forwardOrCall(std::move(optional_arg));
  }

  template <typename TValue>
  inline auto operator()(std::optional<TValue>& optional_arg) {
    return forwardOrCall(optional_arg);
  }

 private:
  template <typename TOptional>
  inline auto forwardOrCall(TOptional&& optional_arg) {
    if (!optional_arg.has_value()) {
      return fallback_fn_();
    }
    return optional_arg.value();
  }

  TFn fallback_fn_;
};

// Helper template function that transforms a given function into a one that is only called in case of fallback. See
// FallbackFn for details.
template <typename TFn>
auto makeFallback(TFn&& fn) {
  return FallbackFn<TFn>(std::forward<TFn>(fn));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_FALLBACK_HPP
