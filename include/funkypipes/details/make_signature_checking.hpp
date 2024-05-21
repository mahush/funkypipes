//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_MAKE_SIGNATURE_CHECKING_HPP
#define FUNKYPIPES_DETAILS_MAKE_SIGNATURE_CHECKING_HPP

#include <type_traits>
#include <utility>

namespace funkypipes::details {

// Helper template struct encapsulating static_asserts for clearer compiler errors. Its template parameters will be
// listed when instantiation fails due to static_asserts.
template <typename TFn, typename TArg>
struct AssertInvocability {
  static_assert(std::is_invocable_v<TFn, TArg>,
                "Callable TFn does not support arguments of type TArg. Look for AssertInvocability in the compiler's "
                "error output to figure out the relevant types TFn and TArg.");
  using ResultType = typename std::invoke_result<TFn, TArg>::type;
  static_assert(not std::is_void_v<ResultType>,
                "Result of TFn may not be void. Look for AssertInvocability in the compiler's error output to figure "
                "out the relevant types TFn and TArg.");
};

// Helper template function that adds static_asserts to the given function to check the signature of the given function.
// Ensures the function:
//  - accepts given arguments
//  - does not return void
template <typename TFn>
auto makeSignatureChecking(TFn&& fn) {
  return [fn = std::forward<TFn>(fn)](auto&& arg) mutable {
    AssertInvocability<TFn, decltype(arg)>{};
    return fn(std::forward<decltype(arg)>(arg));
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_MAKE_SIGNATURE_CHECKING_HPP
