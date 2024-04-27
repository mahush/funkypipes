//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DISABLE_OPTIONAL_ARGUMENT_FN_HPP
#define FUNKYPIPES_DISABLE_OPTIONAL_ARGUMENT_FN_HPP

#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes {

// Helper template to remove support for optional arguments for TFn
template <typename TFn>
struct DisablingOptionalArgumentFn {
  explicit DisablingOptionalArgumentFn(TFn&& fn) : m_fn{std::forward<TFn>(fn)} {}

  template <typename TArg, typename = std::enable_if_t<not details::IsOptional<TArg>::value>>
  auto operator()(TArg&& arg) {
    return m_fn(std::forward<TArg>(arg));
  }

 private:
  TFn m_fn;
};

}  // namespace funkypipes

#endif  // FUNKYPIPES_DISABLE_OPTIONAL_ARGUMENT_FN_HPP
