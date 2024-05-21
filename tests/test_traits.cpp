#include "funkypipes/details/traits.hpp"

using namespace funkypipes::details;

// IsOptional
static_assert(IsOptional<std::optional<int>>::value);
static_assert(not IsOptional<int>::value);

// EnsureOptionalWrapping
static_assert(std::is_same_v<EnsureOptionalWrapping<int>::Type, std::optional<int>>);
static_assert(std::is_same_v<EnsureOptionalWrapping<int&&>::Type, std::optional<int>>);
static_assert(std::is_same_v<EnsureOptionalWrapping<std::optional<int>>::Type, std::optional<int>>);
static_assert(std::is_same_v<EnsureOptionalWrapping<int&>::Type, std::optional<std::reference_wrapper<int>>>);
static_assert(
    std::is_same_v<EnsureOptionalWrapping<const int&>::Type, std::optional<std::reference_wrapper<const int>>>);

// OptionalUnwrapping
static_assert(std::is_same_v<OptionalUnwrapping<std::optional<int>>::Type, int>);
static_assert(std::is_same_v<OptionalUnwrapping<std::optional<std::reference_wrapper<int>>>::Type, int&>);
static_assert(std::is_same_v<OptionalUnwrapping<std::optional<std::reference_wrapper<const int>>>::Type, const int&>);
