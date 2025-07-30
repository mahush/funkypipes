//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_AT_HPP
#define FUNKYPIPES_AT_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "funkypipes/details/make_signature_checking.hpp"
#include "funkypipes/details/tuple/collapse_or_forward_tuple.hpp"
#include "funkypipes/details/tuple/index_sequence.hpp"
#include "funkypipes/details/tuple/resolve_rvalue_references.hpp"
#include "funkypipes/details/tuple/separate_tuple_elements.hpp"
#include "funkypipes/details/tuple/tuple_indices_of.hpp"
#include "funkypipes/details/with_result_tupled.hpp"

namespace funkypipes {

namespace impl {

// Helper function that forwards the arguments of the selected indices to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <typename TFn, typename TProvideSelectedIdxsFn>
auto atImpl(TFn&& fn, TProvideSelectedIdxsFn provideSelectedIdxsFn) {
  using ::funkypipes::details::collapseOrForwardTuple;
  using ::funkypipes::details::makeSignatureChecking;
  using ::funkypipes::details::resolveRValueReferences;
  using ::funkypipes::details::separateTupleElements;
  using ::funkypipes::details::withResultTupled;

  return [tupleReturningFn_ = withResultTupled(makeSignatureChecking(std::forward<TFn>(fn))),
          provideSelectedIdxsFn_ = std::move(provideSelectedIdxsFn)](auto&&... args) mutable -> decltype(auto) {
    auto argsTuple{std::forward_as_tuple(std::forward<decltype(args)>(args)...)};

    auto selectedIdxs = provideSelectedIdxsFn_(argsTuple);

    auto [selectedArgsTuple, otherArgsTuple] = separateTupleElements(std::move(argsTuple), std::move(selectedIdxs));

    auto fnResultTuple = std::apply(tupleReturningFn_, std::move(selectedArgsTuple));

    auto otherArgsTupleWithoutRValueRefs = resolveRValueReferences(std::move(otherArgsTuple));

    auto overallResultTuple = std::tuple_cat(std::move(otherArgsTupleWithoutRValueRefs), std::move(fnResultTuple));

    return collapseOrForwardTuple(std::move(overallResultTuple));
  };
}

}  // namespace impl

// Function decorator that forwards the arguments of the selected indices to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <std::size_t... SelectedIdxs, typename TFn>
auto at(TFn&& fn) {
  using ::funkypipes::impl::atImpl;

  auto provideSelectedIdxsFn = [](const auto& /*argsTuple*/) { return std::index_sequence<SelectedIdxs...>{}; };

  return atImpl(std::forward<TFn>(fn), std::move(provideSelectedIdxsFn));
}

// Function decorator that forwards the arguments of the selected types to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <typename TFirstSelected, typename... TOtherSelected, typename TFn>
auto at(TFn&& fn) {
  using ::funkypipes::details::indexSequenceCat;
  using ::funkypipes::details::TupleIndicesOfAssertingSuccess;
  using ::funkypipes::impl::atImpl;

  auto provideSelectedIdxsFn = [](const auto& argsTuple) {
    using ArgsTuple = std::decay_t<decltype(argsTuple)>;

    return indexSequenceCat(TupleIndicesOfAssertingSuccess<TFirstSelected, ArgsTuple>{},
                            TupleIndicesOfAssertingSuccess<TOtherSelected, ArgsTuple>{}...);
  };

  return atImpl(std::forward<TFn>(fn), std::move(provideSelectedIdxsFn));
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_AT_HPP
