//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_CONSUME_AT_HPP
#define FUNKYPIPES_CONSUME_AT_HPP

#include <tuple>
#include <utility>

#include "funkypipes/details/tuple/separate_element_from_tuple.hpp"
#include "funkypipes/details/tuple/try_flatten_tuple.hpp"

namespace funkypipes {

template <std::size_t Index, typename TFunc>
auto consumeAt(TFunc&& func) {
  return [func = std::forward<TFunc>(func)](auto&&... args) {
    auto tuple{std::make_tuple(std::forward<decltype(args)>(args)...)};

    auto [indexElement, remainingTuple] = funkypipes::details::separateElementFromTuple<Index>(std::move(tuple));

    func(std::forward<decltype(indexElement)>(indexElement));

    return funkypipes::details::tryFlattenTuple(std::forward<decltype(remainingTuple)>(remainingTuple));
  };
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_CONSUME_AT_HPP
