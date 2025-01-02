//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_DETAILS_TUPLE_RESOLVE_RVALUE_REFERENCES_HPP
#define FUNKYPIPES_DETAILS_TUPLE_RESOLVE_RVALUE_REFERENCES_HPP

#include <tuple>
#include <type_traits>
#include <utility>

namespace funkypipes::details {

namespace impl {

// Helper type trait that removes rvalue references
template <typename T>
using RemoveRValueReference = std::conditional_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>, T>;

// This helper function replaces rvalue references with their corresponding value types, transforming the types of the
// tuple elements.
template <typename TTuple, std::size_t... Idxs>
auto resolveRValueReferencesImpl(TTuple&& tuple, std::index_sequence<Idxs...>) {
  using ResultTuple = std::tuple<RemoveRValueReference<std::tuple_element_t<Idxs, std::decay_t<TTuple>>>...>;
  return ResultTuple{std::get<Idxs>(std::forward<TTuple>(tuple))...};
}

}  // namespace impl

// Function transforming a given tuple by replacing rvalue reference elements with value elements.
template <typename... TElements>
auto resolveRValueReferences(std::tuple<TElements...>& tuple) {
  return ::funkypipes::details::impl::resolveRValueReferencesImpl(tuple, std::index_sequence_for<TElements...>());
}

// Function transforming a given tuple by replacing rvalue reference elements with value elements.
template <typename... TElements>
auto resolveRValueReferences(const std::tuple<TElements...>& tuple) {
  return ::funkypipes::details::impl::resolveRValueReferencesImpl(tuple, std::index_sequence_for<TElements...>());
}

// Function transforming a given tuple by replacing rvalue reference elements with value elements.
template <typename... TElements>
auto resolveRValueReferences(std::tuple<TElements...>&& tuple) {
  return ::funkypipes::details::impl::resolveRValueReferencesImpl(std::move(tuple),
                                                                  std::index_sequence_for<TElements...>());
}

}  // namespace funkypipes::details

#endif  // FUNKYPIPES_DETAILS_TUPLE_RESOLVE_RVALUE_REFERENCES_HPP
