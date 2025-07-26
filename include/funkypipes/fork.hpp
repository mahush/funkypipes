//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_FORK_HPP
#define FUNKYPIPES_FORK_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/tuple/separate_tuple_elements.hpp"
#include "funkypipes/details/tuple/try_flatten_tuple.hpp"
#include "funkypipes/details/tuple/tuple_indices_of.hpp"
#include "funkypipes/details/with_result_tupled.hpp"
#include "funkypipes/funky_void.hpp"

namespace funkypipes {

// Function decorator that forwards all arguments to each of the given functions and returns a tuple of their results.
// If possible the result tuple is flattened.
template <typename... TFns>
auto fork(TFns&&... fns) {
  namespace fpd = ::funkypipes::details;

  return [fnsTuple = std::make_tuple(fpd::withResultTupled(fpd::makeSignatureChecking(std::forward<TFns>(fns)))...)](
             auto&&... args) mutable -> decltype(auto) {
    return std::apply(
        [&](auto&&... fns) -> decltype(auto) {
          auto resultTuple = std::tuple_cat(fns(std::as_const(
              args)...)...);  // Note: forward args as const to prevent modification in between calling fns

          using IndexSequencePointingToVoidEntries = fpd::TupleIndicesOf<FunkyVoid, decltype(resultTuple)>;

          auto [voidResults, nonVoidResults] =
              fpd::separateTupleElements(std::move(resultTuple), IndexSequencePointingToVoidEntries{});

          return fpd::tryFlattenTuple(std::move(nonVoidResults));
        },
        fnsTuple);
  };
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_FORK_HPP
