//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include "funkypipes/details/tuple/index_sequence.hpp"

using namespace funkypipes::details;

static_assert(std::is_same_v<IndexSequenceSpan<2, 0>, std::index_sequence<>>);
static_assert(std::is_same_v<IndexSequenceSpan<0, 1>, std::index_sequence<0>>);
static_assert(std::is_same_v<IndexSequenceSpan<0, 2>, std::index_sequence<0, 1>>);
static_assert(std::is_same_v<IndexSequenceSpan<1, 1>, std::index_sequence<1>>);
static_assert(std::is_same_v<IndexSequenceSpan<5, 3>, std::index_sequence<5, 6, 7>>);

static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0, 1>{}, std::index_sequence<3, 4>{})),
                             std::index_sequence<0, 1, 3, 4>>);
static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0, 1>{}, std::index_sequence<0, 1>{})),
                             std::index_sequence<0, 1, 0, 1>>);