//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//
#ifndef FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_FORWARDING_FN_HPP
#define FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_FORWARDING_FN_HPP

struct MoveOnlyForwardingFn {
  MoveOnlyForwardingFn() = default;
  ~MoveOnlyForwardingFn() = default;
  MoveOnlyForwardingFn(const MoveOnlyForwardingFn&) = delete;
  MoveOnlyForwardingFn(MoveOnlyForwardingFn&&) = default;
  MoveOnlyForwardingFn& operator=(const MoveOnlyForwardingFn&) = delete;
  MoveOnlyForwardingFn& operator=(MoveOnlyForwardingFn&&) = delete;

  template <typename TArg>
  TArg operator()(TArg arg) const {
    return arg;
  }
};

#endif  // FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_FORWARDING_FN_HPP
