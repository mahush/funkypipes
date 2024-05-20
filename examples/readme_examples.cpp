
#include <gtest/gtest.h>

#include <sstream>

#include "funkypipes/make_failable_pipe.hpp"

using namespace funkypipes;
using namespace std::string_literals;

TEST(ReadmeExamples, readme_example_basic) {
  auto boolToInt = [](bool flag) -> int { return static_cast<int>(flag); };
  auto toString = [](auto arg) -> std::string { return std::to_string(arg); };
  auto twoTimes = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makeFailablePipe(boolToInt, toString, twoTimes);

  std::optional<std::string> result = pipe(true);

  ASSERT_EQ(result, "11");
}

TEST(ReadmeExamples, readme_example_multiple_parameter) {
  auto generateSomeData = [](int arg1, int arg2, int arg3) -> std::tuple<int, std::string> {
    return {arg1 + arg2 + arg3, "4"};
  };
  auto mergeToString = [](int value, std::string string) { return std::to_string(value) + string; };

  auto pipe = makeFailablePipe(generateSomeData, mergeToString);

  std::optional<std::string> result = pipe(1, 2, 3);

  ASSERT_EQ(result, "64");
}

TEST(ReadmeExamples, readme_example_chain_breaking) {
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };
  auto forward = [](int value) -> int { return value; };

  auto pipe = makeFailablePipe(breakWhenZero, forward, forward);

  std::optional<int> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<int> res2 = pipe(7);  // forwarding case
  EXPECT_EQ(res2, 7);
}

TEST(ReadmeExamples, readme_example_generic) {
  auto forward = [](auto... args) { return std::make_tuple(args...); };
  auto sum = [](auto... args) { return (args + ...); };

  auto pipe = makeFailablePipe(forward, sum);

  ASSERT_EQ(pipe(0), 0);
  ASSERT_EQ(pipe(1, 2, 3), 6);
  ASSERT_EQ(pipe("1"s, "2"s, "3"s), "123"s);
}

TEST(ReadmeExamples, readme_example_reference) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  result.value()++;
  ASSERT_EQ(argument, 2);
}

TEST(ReadmeExamples, readme_example_resursiv) {
  auto forward = [](bool flag) { return flag; };

  auto pipe1 = makeFailablePipe(forward, forward, forward);
  auto pipe2 = makeFailablePipe(pipe1, pipe1, forward);
  auto pipe3 = makeFailablePipe(pipe2, pipe2);

  ASSERT_EQ(pipe3(true), true);
}
