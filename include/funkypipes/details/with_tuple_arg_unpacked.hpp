//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_TUPLE_UNPACKING_HPP
#define FUNKYPIPES_DETAILS_MAKE_TUPLE_UNPACKING_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// Functor that unpacks tuple arguments and calls the given function 'fn' with the unpacked values. When called with
// empty tuples the function 'fn' is called with no arguments. Single arguments are forwarded directly.
template <typename TFn>
class UnpackingFn {
 public:
  explicit UnpackingFn(TFn&& fn) : fn_(std::forward<TFn>(fn)) {}

  template <typename TArg>
  inline auto operator()(TArg&& arg) -> decltype(auto) {
    return fn_(std::forward<TArg>(arg));
  }

  template <typename... Ts>
  inline auto operator()(std::tuple<Ts...>&& arg) -> decltype(auto) {
    return std::apply(fn_, std::move(arg));
  }

 private:
  TFn fn_;
};

// Helper template function that transforms a given function into a one that can unpack tuple arguments. See
// UnpackingFn for details.
template <typename TFn>
auto withTupleArgUnpacked(TFn&& fn) {
  return UnpackingFn<TFn>{std::forward<TFn>(fn)};
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_TUPLE_UNPACKING_HPP
