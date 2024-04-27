//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_COMPOSE_HPP
#define FUNKYPIPES_COMPOSE_HPP

#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// A function that takes any value and ensures it is wrapped in std::optional.
template <typename TArg>
auto ensureOptional(TArg&& arg) {
  using OptionalType = typename EnsureOptional<std::decay_t<TArg>>::Type;
  return OptionalType{std::forward<TArg>(arg)};
}

// Helper function template overload that terminates the recursion. The given callable essentially represents the
// composition. To make sure that the first callable in the chain is always called with an optional argument (as
// expected by the skippable callables) ensureOptional is applied to the argument.
template <typename TFn>
auto composeSkippables(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&& arg) mutable {
    return fn_(ensureOptional(std::forward<decltype(arg)>(arg)));
  };
}

// Helper function template overload that creates a composition out of an arbitrary number of given callables. It
// composes the first two callables into a new lambda that, when called, executes the first callable and passes its
// result to the second callable. The function then recursively composes this
// combined lambda with the rest of the provided callables.
template <typename TFn1, typename TFn2, typename... TFnOthers>
auto composeSkippables(TFn1&& fn_1, TFn2&& fn_2, TFnOthers&&... fn_others) {
  auto chained_fn = [fn_1_ = std::forward<TFn1>(fn_1), fn_2_ = std::forward<TFn2>(fn_2)](auto&& arg) mutable {
    return fn_2_(fn_1_(std::forward<decltype(arg)>(arg)));
  };
  return composeSkippables(std::move(chained_fn), std::forward<TFnOthers>(fn_others)...);
}

}  // namespace funkypipes::details

namespace funkypipes {

// Function template that creates a composition out of the given callables. It makes each callable "skippable" and
// "signature checking", afterwards the callables are composes into a single callable chain using ComposeSkippables.
template <typename... TFns>
auto compose(TFns&&... fns) {
  return composeSkippables(details::makeSkippable(std::forward<TFns>(fns))...);
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_COMPOSE_HPP
