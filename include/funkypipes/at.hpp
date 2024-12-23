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

// Helper trait that ectends TupleIndicesOf by asserting that the requested element was found at least once.
template <typename TRequested, typename TTuple>
struct TupleIndicesOfAssertingSuccessImpl {
  using type = ::funkypipes::details::TupleIndicesOf<TRequested, TTuple>;
  static_assert(type::size() >= 1, "At least one of the selected types is not available");
};
template <typename TRequested, typename TTuple>
using TupleIndicesOfAssertingSuccess = typename TupleIndicesOfAssertingSuccessImpl<TRequested, TTuple>::type;

// Helper function that forwards the arguments of the selected indices to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <typename TFn, typename TTuple, size_t... SelectedIdxs>
auto atImpl(TFn& tupleReturningFn, TTuple argsTuple, std::index_sequence<SelectedIdxs...>) -> decltype(auto) {
  using ::funkypipes::details::resolveRValueReferences;
  using ::funkypipes::details::separateTupleElements;
  using ::funkypipes::details::tryFlattenTuple;

  auto [selectedArgsTuple, otherArgsTuple] = separateTupleElements<SelectedIdxs...>(std::move(argsTuple));

  auto fnResultTuple = std::apply(tupleReturningFn, std::move(selectedArgsTuple));

  auto otherArgsTupleWithoutRValueRefs = resolveRValueReferences(std::move(otherArgsTuple));

  auto overallResultTuple = std::tuple_cat(std::move(otherArgsTupleWithoutRValueRefs), std::move(fnResultTuple));

  return tryFlattenTuple(std::move(overallResultTuple));
}

}  // namespace impl

// Function decorator that forwards the arguments of the selected indices to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <std::size_t... SelectedIdxs, typename TFn>
auto at(TFn&& fn) {
  using ::funkypipes::details::makeSignatureChecking;
  using ::funkypipes::details::makeTupleReturning;
  using ::funkypipes::impl::atImpl;

  return [tupleReturningFn_ = makeTupleReturning(makeSignatureChecking(std::forward<TFn>(fn)))](
             auto&&... args) mutable -> decltype(auto) {
    auto argsTuple{std::forward_as_tuple(std::forward<decltype(args)>(args)...)};
    return atImpl(tupleReturningFn_, std::move(argsTuple), std::index_sequence<SelectedIdxs...>{});
  };
}

// Function decorator that forwards the arguments of the selected types to the given function. Its result is the
// concatenation of the remaining arguments and the function's result.
template <typename TFirstSelected, typename... TOtherSelected, typename TFn>
auto at(TFn&& fn) {
  using ::funkypipes::details::indexSequenceCat;
  using ::funkypipes::details::makeSignatureChecking;
  using ::funkypipes::details::makeTupleReturning;
  using ::funkypipes::impl::atImpl;
  using ::funkypipes::impl::TupleIndicesOfAssertingSuccess;

  return [tupleReturningFn_ = makeTupleReturning(makeSignatureChecking(std::forward<TFn>(fn)))](
             auto&&... args) mutable -> decltype(auto) {
    auto argsTuple{std::forward_as_tuple(std::forward<decltype(args)>(args)...)};
    using ArgsTuple = decltype(argsTuple);

    auto selectedIndices = indexSequenceCat(TupleIndicesOfAssertingSuccess<TFirstSelected, ArgsTuple>{},
                                            TupleIndicesOfAssertingSuccess<TOtherSelected, ArgsTuple>{}...);

    return atImpl(tupleReturningFn_, std::move(argsTuple), std::move(selectedIndices));
  };
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_AT_HPP
