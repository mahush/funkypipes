//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_ARG_OPTIONAL_HPP
#define FUNKYPIPES_DETAILS_MAKE_ARG_OPTIONAL_HPP

#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// Functor wrapping a given function to make their argument wrapped in std::optional unless the argument is already an
// optional, in this case it's just forwarded as is.
template <typename TFn>
class ArgOptionalFn {
 public:
  explicit ArgOptionalFn(TFn&& fn) : fn_(std::forward<TFn>(fn)) {}

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    return fn_(std::move(optional_arg));
  }

  template <typename TValue>
  inline auto operator()(std::optional<TValue>& optional_arg) {
    return fn_(optional_arg);
  }

  template <typename TArg>
  inline auto operator()(TArg&& arg) {
    using OptionalType = typename OptionalWrapping<TArg>::Type;
    return fn_(OptionalType(std::forward<TArg>(arg)));
  }

 private:
  TFn fn_;
};

// Helper function template wrapping a given function fn such that its argument is wrapped in
// std::optional. For details see ArgOptionalFn.
template <typename TFn>
auto makeArgOptional(TFn&& fn) {
  return ArgOptionalFn<TFn>{std::forward<TFn>(fn)};
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_ARG_OPTIONAL_HPP
