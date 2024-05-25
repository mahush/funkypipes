//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github/mahush/funkypipes
//

#include <gtest/gtest.h>

#include "funkypipes/make_failable_pipe.hpp"

using namespace funkypipes;
using namespace funkypipes::details;

int function_1(bool flag) { return flag ? 7 : 0; };
std::string function_2(int value) { return std::to_string(value); };
std::string function_3(std::string string) { return string + string; };

TEST(MakeFailablePipe, free_functions_composition__called_with_lvalue_value__is_executed) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, free_functions_composition__called_with_rvalue_value__is_executed) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  auto result = pipe(true);
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, free_function_composition__called_with_lvalue_optional__is_executed) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  std::optional<bool> flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, free_function_composition__called_with_rvalue_optional__is_executed) {
  auto pipe = makeFailablePipe(function_1, function_2, function_3);

  auto result = pipe(std::optional<bool>{true});
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, lambdas_composition_with_single_argument__called__is_executed) {
  auto lambda_1 = [](bool flag) -> int { return flag ? 7 : 0; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };
  auto lambda_3 = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makeFailablePipe(lambda_1, lambda_2, lambda_3);

  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "77");
}

TEST(MakeFailablePipe, lambdas_composition_with_multiple_arguments__called__is_executed) {
  auto lambda_1 = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  auto result = pipe(1, 2);
  ASSERT_EQ(result, "3");
}

TEST(MakeFailablePipe, lambdas_composition_with_tuple_argument__called__is_executed) {
  auto lambda_1 = [](int arg1, int arg2) -> int { return arg1 + arg2; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  auto result = pipe(std::make_tuple(3, 4));
  ASSERT_EQ(result, "7");
}

TEST(MakeFailablePipe, lambda_composition_with_tuple_passthrough__called__is_executed) {
  auto lambda_1 = [](bool flag) -> std::tuple<int, std::string> { return {flag, "2"}; };
  auto lambda_2 = [](int arg1, const std::string& arg2) -> std::string { return std::to_string(arg1) + arg2; };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  bool flag = true;
  auto result = pipe(flag);
  ASSERT_EQ(result, "12");
}

TEST(MakeFailablePipe, lambda_composition_with_tuple_result__called__is_executed) {
  auto lambda_1 = [](bool flag) -> bool { return static_cast<int>(flag); };
  auto lambda_2 = [](int value) -> std::tuple<int, std::string> { return {value, std::to_string(value)}; };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  bool flag = true;
  auto tuple_result = pipe(flag);

  ASSERT_TRUE(tuple_result.has_value());
  auto [result_1, result_2] = tuple_result.value();
  ASSERT_EQ(result_1, 1);
  ASSERT_EQ(result_2, "1");
}

TEST(MakeFailablePipe, lambdas_composition_without_arguments__called__is_executed) {
  auto lambda_1 = []() -> int { return 0; };
  auto lambda_2 = [](int value) -> std::string { return std::to_string(value); };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  auto result = pipe();
  ASSERT_EQ(result, "0");
}

TEST(MakeFailablePipe, generic_lambdas_composition__called_with_different_types__type_specific_chain_executed) {
  auto generic_lambda = [](auto arg) { return arg; };

  auto pipe = makeFailablePipe(generic_lambda, generic_lambda);

  {
    bool flag = true;
    auto result = pipe(flag);
    ASSERT_EQ(result, true);
  }

  {
    std::string string = "string";
    auto result = pipe(string);
    ASSERT_EQ(result, "string");
  }
}

TEST(MakeFailablePipe, overloaded_functors_composition__called_with_each_type__type_specific_chain_executed) {
  struct OverloadFn1 {
    int operator()(int arg) const { return arg; }
    std::string operator()(std::string arg) const { return arg; }
  };
  struct OverloadFn2 {
    int operator()(int arg) const { return arg; }
    std::string operator()(std::string arg) const { return arg; }
  };

  auto pipe = makeFailablePipe(OverloadFn1{}, OverloadFn2{});

  std::function<std::optional<int>(int)> pipeline1 = makeFailablePipe(OverloadFn1{}, OverloadFn2{});
  std::function<std::optional<std::string>(std::string)> pipeline2 = makeFailablePipe(OverloadFn1{}, OverloadFn2{});

  EXPECT_EQ(0, pipe(0));
  EXPECT_EQ("0", pipe(std::string{"0"}));
}

TEST(MakeFailablePipe, non_copyable_functions_composition__called__is_executed) {
  struct NonCopyableFn {
    NonCopyableFn() = default;
    ~NonCopyableFn() = default;
    NonCopyableFn(const NonCopyableFn&) = delete;
    NonCopyableFn(NonCopyableFn&&) = default;
    NonCopyableFn& operator=(const NonCopyableFn&) = delete;
    NonCopyableFn& operator=(NonCopyableFn&&) = delete;

    int operator()(int value) const { return value; }
  };

  auto pipe = makeFailablePipe(NonCopyableFn{}, NonCopyableFn{}, NonCopyableFn{});
  EXPECT_EQ(0, pipe(0));
}

TEST(MakeFailablePipe, non_copyable_arguments_composition__called__is_executed) {
  struct NonCopyableArg {
    explicit NonCopyableArg(int arg) : m_arg{arg} {}
    ~NonCopyableArg() = default;
    NonCopyableArg(const NonCopyableArg&) = delete;
    NonCopyableArg(NonCopyableArg&&) = default;
    NonCopyableArg& operator=(const NonCopyableArg&) = delete;
    NonCopyableArg& operator=(NonCopyableArg&&) = delete;
    int m_arg;
  };
  auto lambda = [](NonCopyableArg arg) { return arg; };
  auto lambda_optional = [](NonCopyableArg arg) { return std::make_optional<NonCopyableArg>(std::move(arg)); };

  auto pipe = makeFailablePipe(lambda, lambda_optional, lambda);
  auto res = pipe(NonCopyableArg{0});

  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(0, res.value().m_arg);
}

TEST(MakeFailablePipe, lambdas_composition__callable_throws__compostion_passes_through) {
  auto lambda_1 = [](bool) -> int { return 0; };
  auto lambda_2 = [](int) -> bool {
    throw std::exception{};
    return false;
  };

  auto pipe = makeFailablePipe(lambda_1, lambda_2);

  EXPECT_THROW(pipe(false), std::exception);
}

TEST(MakeFailablePipe, lambdas_composition__intermediate_lambda_returns_nullopt__the_chain_breaks) {
  auto breaking_lambda = [](bool) -> std::optional<int> { return std::nullopt; };
  auto subsequent_lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeFailablePipe(breaking_lambda, subsequent_lambda, subsequent_lambda);

  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(true));
  EXPECT_FALSE(res.has_value());
}

TEST(MakeFailablePipe, lambdas_composition__called_with_nullopt__noting_is_executed) {
  auto lambda = [](int) -> int {
    throw std::exception();
    return 0;
  };

  auto pipe = makeFailablePipe(lambda, lambda, lambda);

  std::optional<int> arg{std::nullopt};
  std::optional<int> res;
  EXPECT_NO_THROW(res = pipe(arg));
  EXPECT_FALSE(res.has_value());
}

TEST(MakeFailablePipe, generic_lambdas_composition__assigned_to_std_functions__calling_works) {
  auto generic_lambda = [](auto arg) { return arg; };

  auto pipe = makeFailablePipe(generic_lambda, generic_lambda);
  std::function<std::optional<int>(int)> function_a = pipe;
  std::function<std::optional<std::string>(std::string)> function_b = pipe;

  ASSERT_EQ(1, function_a(1));
  ASSERT_EQ("1", function_b("1"));
}

TEST(MakeFailablePipe, callables_forwarding_refererence__composed__references_are_preserved) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  result.value()++;
  ASSERT_EQ(argument, 2);
}

TEST(MakeFailablePipe, callables_forwarding_const_refererence__composed__const_references_are_preserved) {
  auto lambda = [](const int& value) -> const int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<const int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  argument++;
  ASSERT_EQ(result, 2);
}

// TEST(MakeFailablePipe, void_return_type_callable__compose__triggers_static_assert)
// {
//   auto lambda_1 = [](bool) -> int { return 0; };
//   auto lambda_2 = [](int) -> void {};
//
//   auto pipe = makeFailablePipe(lambda_1, lambda_2);
//
//   pipe(false);
// }

// TEST(MakeFailablePipe,
// missmatching_argument_type_compostion__compose__triggers_static_assert) {
//   auto lambda_returning_bool = [](bool) -> bool { return false; };
//   auto lambda_taking_string = [](std::string) -> int { return 0; };
//
//   auto pipe = makeFailablePipe(lambda_returning_bool, lambda_taking_string);
//
//   pipe(false);
// }

