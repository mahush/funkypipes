//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_TUPLE_TRAITS_HPP
#define FUNKYPIPES_DETAILS_TUPLE_TUPLE_TRAITS_HPP

#include <tuple>

namespace funkypipes::details {

namespace impl {

template <typename T>
struct IsTupleImpl : std::false_type {};
template <typename... T>
struct IsTupleImpl<std::tuple<T...>> : std::true_type {};

}  // namespace impl

// A type trait that checks if a given type is std::tuple
template <typename T>
constexpr bool IsTuple = ::funkypipes::details::impl::IsTupleImpl<T>::value;

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_TUPLE_TRAITS_HPP
