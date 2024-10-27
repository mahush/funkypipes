
#include <gtest/gtest.h>

#include <sstream>

#include "funkypipes/make_auto_pipe.hpp"

using namespace funkypipes;
using namespace std::string_literals;

TEST(ReadmeExamples, readme_example_basic) {
  auto boolToInt = [](bool flag) -> int { return static_cast<int>(flag); };
  auto toString = [](auto arg) -> std::string { return std::to_string(arg); };
  auto twoTimes = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makeAutoPipe(boolToInt, toString, twoTimes);

  std::string result = pipe(true);

  ASSERT_EQ(result, "11");
}

TEST(ReadmeExamples, readme_example_multiple_parameter) {
  auto generateSomeData = [](int arg1, int arg2, int arg3) -> std::tuple<int, std::string> {
    return {arg1 + arg2 + arg3, "4"};
  };
  auto mergeToString = [](int value, std::string string) { return std::to_string(value) + string; };

  auto pipe = makeAutoPipe(generateSomeData, mergeToString);

  std::string result = pipe(1, 2, 3);

  ASSERT_EQ(result, "64");
}

TEST(ReadmeExamples, readme_example_chain_breaking) {
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };
  auto forward = [](int value) -> int { return value; };

  auto pipe = makeAutoPipe(breakWhenZero, forward, forward);

  std::optional<int> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<int> res2 = pipe(7);  // forwarding case
  EXPECT_EQ(res2, 7);
}

TEST(ReadmeExamples, readme_example_generic) {
  auto forward = [](auto... args) { return std::make_tuple(args...); };
  auto sum = [](auto... args) { return (args + ...); };

  auto pipe = makeAutoPipe(forward, sum);

  ASSERT_EQ(pipe(0), 0);
  ASSERT_EQ(pipe(1, 2, 3), 6);
  ASSERT_EQ(pipe("1"s, "2"s, "3"s), "123"s);
}

TEST(ReadmeExamples, readme_example_reference) {
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeAutoPipe(lambda, lambda);

  int argument{1};
  int& result = pipe(argument);
  ASSERT_EQ(result, 1);

  result++;
  ASSERT_EQ(argument, 2);
}

TEST(ReadmeExamples, readme_example_nested) {
  auto increment = [](int value) {
    ++value;
    return value;
  };

  auto pipe1 = makeAutoPipe(increment, increment, increment);
  auto pipe2 = makeAutoPipe(pipe1, pipe1, increment);
  auto pipe3 = makeAutoPipe(pipe2, pipe2);

  ASSERT_EQ(pipe3(0), 14);
}
