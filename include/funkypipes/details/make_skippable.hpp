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

// A type trait providing the return type of TFn including compile-time checks
template <typename TFn, typename TArg>
struct InvokeResult {
  static_assert(std::is_invocable_v<TFn, TArg>, "Callable TFn does not support arguments of type TArg");
  static_assert(not std::is_invocable_v<TFn, std::optional<TArg>>,
                "Callable TFn may not support an argument of type std::optional");

  using Type = typename std::invoke_result<TFn, TArg>::type;
  static_assert(not std::is_void_v<Type>, "Result of TFn may not be void");
};

// Helper template function that transforms a given function (Fn) into another one (FnSkippable). FnSkippable expectes
// Fn's argument wrapped in a std::optional. Moreover, FnSkippable returns Fn's result wrapped into std::optional,
// unless it is not already. When calling FnSkippable, Fn is just executed unless FnSkippable is called with a
// std::nullopt. In this case, Fn is not executed (skipped) and FnSkippable simply returns a std::nullopt.
template <typename TFn>
auto makeSkippable(TFn&& fn) {
  return [fn = std::forward<TFn>(fn)](auto&& optional_arg) mutable {
    using OptionalArg = std::decay_t<decltype(optional_arg)>;
    using ValueArg = typename OptionalArg::value_type;
    using FnResult = typename InvokeResult<TFn, ValueArg>::Type;
    using OptionalFnResult = typename EnsureOptional<FnResult>::Type;

    if (optional_arg.has_value()) {
      auto&& unwrapped_value = std::forward<OptionalArg>(optional_arg).value();
      return OptionalFnResult{fn(std::forward<decltype(unwrapped_value)>(unwrapped_value))};
    }

    // skip fn
    return OptionalFnResult{std::nullopt};
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_SKIPPABLE_HPP
