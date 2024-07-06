//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_POSSIBLY_SKIPPABLE_HPP
#define FUNKYPIPES_DETAILS_MAKE_POSSIBLY_SKIPPABLE_HPP

#include "funkypipes/details/make_skippable.hpp"

namespace funkypipes::details {

// This functor extends `SkippableFn` that requires optional arguments (case 1) by an overload for non-optional
// arguments (case 2). This way (depended on the argument type) it's possible that `fn` is not skippable.
//
//  - Case 1: Handles optional arguments (implemented by base class SkippableFn)
//
//    'fn' might be skipped. If called with std::nullopt, the functor skips the execution of 'fn' and directly returns
//    std::nullopt. Otherwise, it executes 'fn' and wraps the result in std::optional unless it is already an
//    std::optional. So, it's guaranteed that a std::optional is returned.
//
//  - Case 2: Handles non-optional argument (implemented in derived class PossiblySkippableFn)
//
//    'fn' is not skipped. The given argument is passed to 'fn' and the result of 'fn' is returned directly. Thus, a
//    std::optional might or might not be returned.
//
template <typename TFn>
class PossiblySkippableFn : public SkippableFn<TFn> {
 public:
  explicit PossiblySkippableFn(TFn&& fn) : SkippableFn<TFn>{std::forward<TFn>(fn)} {}

  // Note: The following using declaration is necessary because defining the function template below hides the
  // base class's operator() templates. By bringing the base class's operator() templates into the scope of the
  // derived class, we ensure that std::optional arguments are correctly handled by the inherited implementations.
  using SkippableFn<TFn>::operator();  // Case 1: 'fn' might be skipped

  template <typename TArg>
  inline auto operator()(TArg&& arg) -> decltype(auto) {  // Case 2: 'fn' is not skipped
    return SkippableFn<TFn>::callFn(std::forward<TArg>(arg));
  }
};

// Helper template function that transforms a given function into a skippable one. See PossiblySkippableFn for details.
template <typename TFn>
auto makePossiblySkippable(TFn&& fn) {
  return PossiblySkippableFn<TFn>(std::forward<TFn>(fn));
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_POSSIBLY_SKIPPABLE_HPP
