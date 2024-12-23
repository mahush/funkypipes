//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_AT_HPP
#define FUNKYPIPES_AT_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/make_tuple_returning.hpp"
#include "funkypipes/details/tuple/separate_tuple_elements.hpp"
#include "funkypipes/details/tuple/try_flatten_tuple.hpp"

namespace funkypipes {

// Function decorator that forwards that forwards the arguments of the selected indices to the given function. The
// result is the concatenation of the remaining arguments and the functions result.
template <std::size_t... SelectedIdxs, typename TFn>
auto at(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&&... args) mutable {
    using ::funkypipes::details::separateTupleElements;
    using ::funkypipes::details::makeTupleReturning;
    using ::funkypipes::details::tryFlattenTuple;

    auto argsTuple{std::make_tuple(std::forward<decltype(args)>(args)...)};

    auto [selectedArgsTuple, otherArgsTuple] = separateTupleElements<SelectedIdxs...>(std::move(argsTuple));

    auto tupleReturningFn = makeTupleReturning(std::forward<TFn>(fn_));

    auto fnResultTuple = std::apply(std::move(tupleReturningFn), std::move(selectedArgsTuple));

    auto overallResultTuple = std::tuple_cat(std::move(otherArgsTuple), std::move(fnResultTuple));

    return tryFlattenTuple(std::move(overallResultTuple));
  };
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_AT_HPP
