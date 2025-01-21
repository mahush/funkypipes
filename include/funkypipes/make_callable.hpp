//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#ifndef FUNKYPIPES_MAKE_CALLABLE_HPP
#define FUNKYPIPES_MAKE_CALLABLE_HPP

#include <utility>

// This macro wraps a specified invocable expression into a lambda function. The resulting lambda can be invoked with
// any number of arguments, which are then perfectly forwarded to the original invocable expression. The lambda returns
// the result of this invocation.
#define FUNKYPIPES_MAKE_CALLABLE(invokable_expression)                                                                \
  [&](auto&&... funkypipesMakeCallableArgs) {                                                                         \
    return (invokable_expression)(std::forward<decltype(funkypipesMakeCallableArgs)>(funkypipesMakeCallableArgs)...); \
  }

#ifdef MAKE_CALLABLE
#pragma message("Warning: MAKE_CALLABLE is already defined. Consider using FUNKYPIPES_MAKE_CALLABLE instead.")
#else
#define MAKE_CALLABLE FUNKYPIPES_MAKE_CALLABLE
#endif

#endif  // FUNKYPIPES_MAKE_CALLABLE_HPP
