//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_PASS_ALONG_HPP
#define FUNKYPIPES_PASS_ALONG_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/make_tuple_returning.hpp"
#include "funkypipes/details/tuple/index_sequence.hpp"
#include "funkypipes/details/tuple/resolve_rvalue_references.hpp"
#include "funkypipes/details/tuple/separate_tuple_elements.hpp"
#include "funkypipes/details/tuple/try_flatten_tuple.hpp"
#include "funkypipes/details/tuple/tuple_indices_of.hpp"

namespace funkypipes {

namespace impl {

template <typename TFn, typename TProvidePassAlongArgsFn>
auto passAlongImpl(TFn&& fn, TProvidePassAlongArgsFn providePassAlongArgsFn) {
  namespace fpd = ::funkypipes::details;

  return [tupleReturningFn_ = fpd::makeTupleReturning(fpd::makeSignatureChecking(std::forward<TFn>(fn))),
          providePassAlongArgsFn_ = std::move(providePassAlongArgsFn)](auto&&... args) mutable -> decltype(auto) {
    auto argsTuple = fpd::resolveRValueReferences(
        std::forward_as_tuple(std::forward<decltype(args)>(args)...));  // Note: lvalue references are preserved

    auto fnResultTuple = std::apply(tupleReturningFn_, argsTuple);

    auto passAlongArgsTuple = providePassAlongArgsFn_(std::move(argsTuple));

    auto overallResultTuple = std::tuple_cat(std::move(fnResultTuple), std::move(passAlongArgsTuple));

    return fpd::tryFlattenTuple(std::move(overallResultTuple));
  };
}

}  // namespace impl

// A decorator function that duplicates the specified input arguments and passes the duplicates along. The decorated
// function processes all input arguments as usual. Finally the decorator function returns both the duplicated arguments
// and the output of the decorated function.
template <typename TFirstPassAlong, typename... TOtherPassAlong, typename TFn>
auto passAlong(TFn&& fn) {
  namespace fpi = ::funkypipes::impl;
  namespace fpd = ::funkypipes::details;

  auto providePassAlongArgs = [](auto argsTuple) {
    using ArgsTuple = std::decay_t<decltype(argsTuple)>;
    auto selectedIndices = fpd::indexSequenceCat(fpd::TupleIndicesOfAssertingSuccess<TFirstPassAlong, ArgsTuple>{},
                                                 fpd::TupleIndicesOfAssertingSuccess<TOtherPassAlong, ArgsTuple>{}...);

    return fpd::recreateTupleFromIndices(std::move(argsTuple), selectedIndices);
  };
  return fpi::passAlongImpl(std::forward<TFn>(fn), std::move(providePassAlongArgs));
}

// A decorator function that duplicates the specified input arguments and passes the duplicates along. The decorated
// function processes all input arguments as usual. Finally the decorator function returns both the duplicated arguments
// and the output of the decorated function.
template <std::size_t FirstPassAlongIdx, std::size_t... OtherPassAlongIdxs, typename TFn>
auto passAlong(TFn&& fn) {
  namespace fpi = ::funkypipes::impl;

  auto providePassAlongArgs = [](auto argsTuple) {
    return ::funkypipes::details::recreateTupleFromIndices<FirstPassAlongIdx, OtherPassAlongIdxs...>(
        std::move(argsTuple));
  };
  return fpi::passAlongImpl(std::forward<TFn>(fn), std::move(providePassAlongArgs));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_PASS_ALONG_HPP
