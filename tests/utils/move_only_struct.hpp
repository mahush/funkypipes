//
// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//
#ifndef FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_STRUCT_HPP
#define FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_STRUCT_HPP

struct MoveOnlyStruct {
  explicit MoveOnlyStruct(int value) : value_(value) {}
  ~MoveOnlyStruct() = default;
  MoveOnlyStruct(const MoveOnlyStruct&) = delete;
  MoveOnlyStruct(MoveOnlyStruct&&) = default;
  MoveOnlyStruct& operator=(const MoveOnlyStruct&) = delete;
  MoveOnlyStruct& operator=(MoveOnlyStruct&&) = delete;

  int value_;  // NOLINT public visibility is intended here
};

#endif  // FUNKYPIPES_TESTS_UTILS_MOVE_ONLY_STRUCT_HPP
