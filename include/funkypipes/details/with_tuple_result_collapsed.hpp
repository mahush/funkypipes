//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_WITH_TUPLE_RESULT_COLLAPSED_HPP
#define FUNKYPIPES_DETAILS_WITH_TUPLE_RESULT_COLLAPSED_HPP

#include <utility>

#include "funkypipes/details/tuple/collapse_or_forward_tuple.hpp"

namespace funkypipes::details {

// Helper template function that decorates a given function by collapsing its result if a tuple that contains one or
// zero elements.
template <typename TFn>
auto withTupleResultCollapsed(TFn&& fn) {
  return [fn_ = std::forward<TFn>(fn)](auto&& arg) mutable -> decltype(auto) {
    return collapseOrForwardTuple(fn_(std::forward<decltype(arg)>(arg)));
  };
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_WITH_TUPLE_RESULT_COLLAPSED_HPP
