//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP
#define FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP

#include "funkypipes/details/make_arg_optional.hpp"
#include "funkypipes/details/make_raw_pipe.hpp"
#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/make_skippable.hpp"
#include "funkypipes/details/make_tuple_packing.hpp"
#include "funkypipes/details/make_tuple_unpacking.hpp"
#include "funkypipes/details/traits.hpp"

namespace funkypipes {
// Function template that creates a pipe out of the given callables. The pipe returns the result wrapped in
// std::optional. On success this are the return values of the last callable in the chain. On failure, triggered by any
// callable returning std::nullopt, it's a std::nullopt according to the return value type of the last callable in the
// chain.
//
// On a detailed level, each callable is decorated to be "skippable", "tuple unpacking" and "signature checking",
// afterwards the callables are composes into a single callable chain using makeRawPipe. Finally the pipe is decorated
// to be "tuple packing" and "argument optional".
template <typename... TFns>
auto makeFailablePipe(TFns&&... fns) {
  using namespace details;
  return makeTuplePacking(makeArgOptional(
      makeRawPipe(makeSkippable(makeTupleUnpacking(makeSignatureChecking(std::forward<TFns>(fns))))...)));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_MAKE_FAILABLE_PIPE_HPP
