//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TRAITS_HPP
#define FUNKYPIPES_DETAILS_TRAITS_HPP

#include <optional>
#include <type_traits>

namespace funkypipes::details {

// A type trait that checks if a given type is std::optional
template <typename T>
struct IsOptional : std::false_type {};
template <typename T>
struct IsOptional<std::optional<T>> : std::true_type {};

// A type trait that wraps a given type in std::optional if it isn't already.
template <typename T>
struct EnsureOptionalWrapping {
  using Type = std::optional<T>;
};
template <typename U>
struct EnsureOptionalWrapping<std::optional<U>> {
  using Type = std::optional<U>;
};

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TRAITS_HPP
