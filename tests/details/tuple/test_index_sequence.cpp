//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include "funkypipes/details/tuple/index_sequence.hpp"

using funkypipes::details::indexSequenceCat;
using funkypipes::details::ComplementIndexSequence;

static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0>{})), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0>{}, std::index_sequence<>{})),
                             std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0, 1>{}, std::index_sequence<3, 4>{})),
                             std::index_sequence<0, 1, 3, 4>>);
static_assert(std::is_same_v<decltype(indexSequenceCat(std::index_sequence<0, 1>{}, std::index_sequence<0, 1>{})),
                             std::index_sequence<0, 1, 0, 1>>);

static_assert(std::is_same_v<ComplementIndexSequence<4, 1, 3>, std::index_sequence<0, 2>>);
static_assert(std::is_same_v<ComplementIndexSequence<2>, std::index_sequence<0, 1>>);
static_assert(std::is_same_v<ComplementIndexSequence<2, 0, 1>, std::index_sequence<>>);
static_assert(std::is_same_v<ComplementIndexSequence<0>, std::index_sequence<>>);
