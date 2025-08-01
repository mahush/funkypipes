//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_MAKE_AUTO_PIPE_HPP
#define FUNKYPIPES_MAKE_AUTO_PIPE_HPP

#include "funkypipes/details/make_possibly_skippable.hpp"
#include "funkypipes/details/make_raw_pipe.hpp"
#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/traits.hpp"
#include "funkypipes/details/with_empty_tuple_result_as_void.hpp"
#include "funkypipes/details/with_non_single_args_tupled.hpp"
#include "funkypipes/details/with_tuple_arg_unpacked.hpp"
#include "funkypipes/details/with_void_result_as_empty_tuple.hpp"

namespace funkypipes {
// Function template that creates a pipe out of the given callables.
//  - Any callable may return a std::optional to be able to break the chain.
//  - No callable should take a std::optional as input because if a std::nullopt is encountered, it will not be
//    forwarded to the callable; instead, the callable will be skipped.
//  - The pipe's return type is the return type of the last given callable, unless any preceding callable in the chain
//    returns a std::optional. In that case, the return type is the last callable's return type wrapped in a
//    std::optional.
//  - In case the last callable's return type is void and needs to be wrapped in a std::optional, not
//    std::optional<void> is returned but std::optional<FunkyVoid>.
//  - If a preceding callable provides a std::optional with a value, it is automatically extracted before being
//    forwarded to the subsequent callable. If a preceding callable provides a std::nullopt, all subsequent callables
//    are skipped.
//
// On a detailed level, each callable is decorated to be "possibly skippable", "return void as empty tuple", "tuple
// unpacking" and "signature checking", afterwards the callables are composes into a single callable chain using
// makeRawPipe. Finally the pipe is decorated to be "tuple packing" and "return empty tuple as void".
template <typename... TFns>
auto makeAutoPipe(TFns&&... fns) {
  using namespace details;
  return withNonSingleArgsTupled(withEmptyTupleResultAsVoid(makeRawPipe(makePossiblySkippable(
      withVoidResultAsEmptyTuple(withTupleArgUnpacked(makeSignatureChecking(std::forward<TFns>(fns)))))...)));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_MAKE_AUTO_PIPE_HPP
