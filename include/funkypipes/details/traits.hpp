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
  using QVQualifiedValueType =
      std::conditional_t<std::is_lvalue_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>,
                         std::remove_reference_t<T>>;
  using Type = std::optional<QVQualifiedValueType>;
};
template <typename U>
struct EnsureOptionalWrapping<std::optional<U>> {
  using Type = std::optional<U>;
};

// A type trait that unwraps the stored type of a given std::optional
template <typename TOptional>
struct OptionalUnwrapping;
template <typename TCVQualified>
struct OptionalUnwrapping<std::optional<TCVQualified>> {
  using Type = TCVQualified;
};
template <typename TCVQualified>
struct OptionalUnwrapping<std::optional<std::reference_wrapper<TCVQualified>>> {
  using Type = std::add_lvalue_reference_t<TCVQualified>;
};

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TRAITS_HPP
