//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_FORK_AND_BYPASS_HPP
#define FUNKYPIPES_FORK_AND_BYPASS_HPP

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

template <typename TFn, typename TProvideForkArgsFn>
auto forkAndBypassImpl(TFn&& fn, TProvideForkArgsFn provideForkArgsFn) {
  namespace fpd = ::funkypipes::details;

  return [tupleReturningFn_ = fpd::makeTupleReturning(fpd::makeSignatureChecking(std::forward<TFn>(fn))),
          provideForkArgsFn_ = std::move(provideForkArgsFn)](auto&&... args) mutable -> decltype(auto) {
    auto argsTuple = fpd::resolveRValueReferences(
        std::forward_as_tuple(std::forward<decltype(args)>(args)...));  // Note: lvalue references are preserved

    auto fnResultTuple = std::apply(tupleReturningFn_, argsTuple);

    auto forkedArgsTuple = provideForkArgsFn_(std::move(argsTuple));

    auto overallResultTuple = std::tuple_cat(std::move(fnResultTuple), std::move(forkedArgsTuple));

    return fpd::tryFlattenTuple(std::move(overallResultTuple));
  };
}

}  // namespace impl

template <typename TFn>
auto forkAndBypass(TFn&& fn) {
  namespace fpi = ::funkypipes::impl;

  auto provideForkArgs = [](auto argsTuple) { return argsTuple; };
  return fpi::forkAndBypassImpl(std::forward<TFn>(fn), std::move(provideForkArgs));
}

template <typename TFirstSelected, typename... TOtherSelected, typename TFn>
auto forkAndBypass(TFn&& fn) {
  namespace fpi = ::funkypipes::impl;
  namespace fpd = ::funkypipes::details;

  auto provideForkArgs = [](auto argsTuple) {
    using ArgsTuple = std::decay_t<decltype(argsTuple)>;
    auto selectedIndices = fpd::indexSequenceCat(fpd::TupleIndicesOfAssertingSuccess<TFirstSelected, ArgsTuple>{},
                                                 fpd::TupleIndicesOfAssertingSuccess<TOtherSelected, ArgsTuple>{}...);

    return fpd::recreateTupleFromIndices(std::move(argsTuple), selectedIndices);
  };
  return fpi::forkAndBypassImpl(std::forward<TFn>(fn), std::move(provideForkArgs));
}

template <std::size_t FirstForkIdx, std::size_t... OtherForkIdxs, typename TFn>
auto forkAndBypass(TFn&& fn) {
  namespace fpi = ::funkypipes::impl;

  auto provideForkArgs = [](auto argsTuple) {
    return ::funkypipes::details::recreateTupleFromIndices<FirstForkIdx, OtherForkIdxs...>(std::move(argsTuple));
  };
  return fpi::forkAndBypassImpl(std::forward<TFn>(fn), std::move(provideForkArgs));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_FORK_AND_BYPASS_HPP
