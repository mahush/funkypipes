//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include "funkypipes/details/tuple/tuple_traits.hpp"

using funkypipes::details::IsTuple;

static_assert(not IsTuple<int>);
static_assert(IsTuple<std::tuple<>>);
static_assert(IsTuple<std::tuple<int>>);
static_assert(IsTuple<std::tuple<int, double>>);

