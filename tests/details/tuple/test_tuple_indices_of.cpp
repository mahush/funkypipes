//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <tuple>

#include "funkypipes/details/tuple/tuple_indices_of.hpp"

using funkypipes::details::TupleIndicesOf;

static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<int>>, std::index_sequence<0>>);
static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<int&>>, std::index_sequence<0>>);
static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<const int&>>, std::index_sequence<0>>);
static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<char>>, std::index_sequence<>>);
static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<int, char, double, int>>, std::index_sequence<0, 3>>);
static_assert(std::is_same_v<TupleIndicesOf<int, std::tuple<>>, std::index_sequence<>>);
