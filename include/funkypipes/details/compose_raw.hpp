//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_COMPOSE_RAW_HPP
#define FUNKYPIPES_DETAILS_COMPOSE_RAW_HPP

#include <utility>

namespace funkypipes::details {

// Helper function template overload that terminates the recursion. The given callable essentially represents the
// composition.
template <typename TFn>
auto composeRaw(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&& arg) mutable { return fn_(std::forward<decltype(arg)>(arg)); };
}

// Helper function template overload that creates a composition out of an arbitrary number of given callables. It
// composes the first two callables into a new lambda that, when called, executes the first callable and passes its
// result to the second callable. The function then recursively composes this
// combined lambda with the rest of the provided callables.
template <typename TFn1, typename TFn2, typename... TFnOthers>
auto composeRaw(TFn1&& fn_1, TFn2&& fn_2, TFnOthers&&... fn_others) {
  auto chaining_fn = [fn_1_ = std::forward<TFn1>(fn_1), fn_2_ = std::forward<TFn2>(fn_2)](auto&& arg) mutable {
    return fn_2_(fn_1_(std::forward<decltype(arg)>(arg)));
  };
  return composeRaw(std::move(chaining_fn), std::forward<TFnOthers>(fn_others)...);
}
}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_COMPOSE_RAW_HPP
