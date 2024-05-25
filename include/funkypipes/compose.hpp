//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_COMPOSE_HPP
#define FUNKYPIPES_COMPOSE_HPP

#include "funkypipes/details/compose_raw.hpp"
#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/traits.hpp"

namespace funkypipes::details {

// A function that takes any value and ensures it is wrapped in std::optional.
template <typename TArg>
auto ensureOptional(TArg&& arg) {
  using OptionalType = typename EnsureOptional<std::decay_t<TArg>>::Type;
  return OptionalType{std::forward<TArg>(arg)};
}

// Helper function template to decorates the composition in order to ensure that the first callable in
// the chain is always called with an optional argument (as expected by the skippable callables).
// Furthermore, to support the composition to be called with zero or multiple parameters. Therefore the
// returned lambda accepts its args as parameter pack, applies ensureOptional and potentionally combines them as tuple.
template <typename TFn>
auto extendCallability(TFn&& fn) {
  return
      [fn = std::forward<TFn>(fn)](auto&& arg) mutable { return fn(ensureOptional(std::forward<decltype(arg)>(arg))); };
}
}  // namespace funkypipes::details

namespace funkypipes {

// Function template that creates a composition out of the given callables. Each callable is decorated to be
// "skippable" and "signature checking", afterwards the callables are composes into a single callable
// chain using composeRaw. Finally the callable chain is decorated to have extended call ability.
template <typename... TFns>
auto compose(TFns&&... fns) {
  using namespace details;
  return extendCallability(composeRaw(makeSkippable(makeSignatureChecking(std::forward<TFns>(fns)))...));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_COMPOSE_HPP
