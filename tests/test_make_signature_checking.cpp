//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/details/make_signature_checking.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

TEST(MakeSignatureChecking, callable__called_with_lvalue__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto signature_checking_lambda = makeSignatureChecking(lambda);

  int argument{1};
  std::string res = signature_checking_lambda(argument);
  EXPECT_EQ(res, "1");
}

TEST(MakeSignatureChecking, callable__called_with_rvalue__works) {
  auto lambda = [](int value) { return std::to_string(value); };

  auto signature_checking_lambda = makeSignatureChecking(lambda);

  int argument{1};
  std::string res = signature_checking_lambda(std::move(argument));
  EXPECT_EQ(res, "1");
}

TEST(MakeSignatureChecking, callable__called_with_non_copyable_value__works) {
  struct NonCopyableArg {
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
  };

  auto lambda = [](NonCopyableArg arg) { return arg; };

  auto signature_checking_lambda = makeSignatureChecking(lambda);

  NonCopyableArg argument{};
  NonCopyableArg res = signature_checking_lambda(std::move(argument));
}

TEST(MakeSignatureChecking, callable__called_without_parameter__works) {
  auto lambda = []() { return "result"; };

  auto signature_checking_lambda = makeSignatureChecking(lambda);

  std::string res = signature_checking_lambda();
  EXPECT_EQ(res, "result");
}

TEST(MakeSignatureChecking, non_copyable_callable__called__works) {
  struct NonCopyableFn {
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto skippable_fn = makeSignatureChecking(NonCopyableFn{});

  int argument{1};
  int res = skippable_fn(argument);
  EXPECT_EQ(res, 1);
}

// TEST(MakeSignatureChecking, callable__called_with_unsupported_argument__triggers_static_assert) {
//   auto lambda = [](int value) { return value; };
//
//   auto signature_checking_lambda = makeSignatureChecking(lambda);
//
//   struct NotAnInt {};
//   NotAnInt argument{};
//   signature_checking_lambda(argument);
// }
