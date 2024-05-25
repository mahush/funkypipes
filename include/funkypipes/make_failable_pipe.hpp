//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP
#define FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP

#include "funkypipes/details/make_raw_pipe.hpp"
#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/make_tuple_unpacking.hpp"
#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// A function that takes any value and ensures it is wrapped in std::optional rvalue.
template <typename TArg>
auto ensureOptional(TArg&& arg) {
  using OptionalType = typename EnsureOptionalWrapping<TArg>::Type;
  return OptionalType{std::forward<TArg>(arg)};
}

// Helper function template to decorates the composition in order to ensure that the first callable in the chain is
// always called with an optional argument as expected by the skippable callables (via ensureOptional).
// Furthermore, support for calling the composition with zero or multiple parameters is added (via args as parameter
// pack and tuple packing).
template <typename TFn>
class CallabilityExtendingDecoratedFn {
 public:
  explicit CallabilityExtendingDecoratedFn(TFn&& fn) : fn_(std::forward<TFn>(fn)) {}

  template <typename TValue>
  inline auto operator()(std::optional<TValue>&& optional_arg) {
    return fn_(std::move(optional_arg));
  }

  template <typename TValue>
  inline auto operator()(std::optional<TValue>& optional_arg) {
    return fn_(optional_arg);
  }

  template <typename... TArgs>
  inline auto operator()(TArgs&&... args) {
    constexpr size_t args_count = sizeof...(args);
    if constexpr (args_count == 1) {
      // Note: single arguments are forwarded directly
      return fn_(ensureOptional(std::forward<TArgs>(args))...);
    } else {
      // Note: zero or multiple arguments are forwarded via tuple
      return fn_(ensureOptional(std::make_tuple(std::forward<decltype(args)>(args)...)));
    }
  }

 private:
  TFn fn_;
};

template <typename TFn>
auto extendCallability(TFn&& fn) {
  return CallabilityExtendingDecoratedFn<TFn>{std::forward<TFn>(fn)};
}

}  // namespace funkypipes::details

namespace funkypipes {

// Function template that creates a composition out of the given callables. Each callable is decorated to be
// "skippable", "tuple unpacking" and "signature checking", afterwards the callables are composes into a single callable
// chain using composeRaw. Finally the callable chain is decorated to have extended call ability.
template <typename... TFns>
auto makeFailablePipe(TFns&&... fns) {
  using namespace details;
  return extendCallability(
      makeRawPipe(makeSkippable(makeTupleUnpacking(makeSignatureChecking(std::forward<TFns>(fns))))...));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP
