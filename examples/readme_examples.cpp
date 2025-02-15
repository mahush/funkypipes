#include <gtest/gtest.h>

#include <string>
#include <tuple>

#include "funkypipes/at.hpp"
#include "funkypipes/bind_front.hpp"
#include "funkypipes/fork.hpp"
#include "funkypipes/make_auto_pipe.hpp"
#include "funkypipes/make_callable.hpp"
#include "funkypipes/make_pipe.hpp"
#include "funkypipes/pass_along.hpp"

using namespace funkypipes;
using namespace std::string_literals;

TEST(ReadmeExamples, make_pipe_basic) {
  auto classifyTemperature = [](int temperature) -> std::tuple<bool, std::string> {
    bool is_alert = (temperature > 42);
    std::string temperatureInfo = "Temperature=" + std::to_string(temperature);
    return {is_alert, temperatureInfo};
  };

  auto swapArgs = [](auto arg1, auto arg2) { return std::make_tuple(arg2, arg1); };

  auto generateLogEntry = [](const std::string& message, bool is_alert) {
    return (is_alert ? "ALERT: " : "Info: ") + message;
  };

  auto generateTemperatureLogEnty = makePipe(classifyTemperature, swapArgs, generateLogEntry);

  ASSERT_EQ(generateTemperatureLogEnty(30), "Info: Temperature=30");
  ASSERT_EQ(generateTemperatureLogEnty(50), "ALERT: Temperature=50");
}

TEST(ReadmeExamples, make_pipe_reference) {
  auto forwardReference = [](bool& value) -> bool& { return value; };

  auto pipe = makePipe(forwardReference, forwardReference);

  bool argument{true};
  bool& result = pipe(argument);
  ASSERT_EQ(result, true);

  result = false;
  ASSERT_EQ(argument, false);
}

TEST(ReadmeExamples, make_pipe_nested) {
  auto increment = [](int value) {
    ++value;
    return value;
  };

  auto pipe1 = makePipe(increment, increment);
  auto pipe2 = makePipe(pipe1, increment);
  auto pipe3 = makePipe(pipe2, pipe2);

  ASSERT_EQ(pipe3(0), 6);
}

TEST(ReadmeExamples, make_pipe_chain_breaking) {
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };

  auto forward = [](int value) { return value; };

  auto toString = [](int value) { return std::to_string(value); };

  auto pipe = makePipe(breakWhenZero, andThen(forward), andThen(toString));

  std::optional<std::string> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<std::string> res2 = pipe(1);  // forwarding case
  EXPECT_EQ(res2, "1");
}

TEST(ReadmeExamples, make_auto_pipe_chain_breaking) {
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };
  auto forward = [](int value) -> int { return value; };

  auto toString = [](int value) { return std::to_string(value); };

  auto pipe = makeAutoPipe(breakWhenZero, forward, toString);

  std::optional<std::string> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<std::string> res2 = pipe(2);  // forwarding case
  EXPECT_EQ(res2, "2");
}

TEST(ReadmeExamples, bind_front) {
  auto greet = [](const std::string& salutation, const std::string& name) { return salutation + " " + name + "!"; };

  auto greetWithHello = bindFront(greet, "Hello");

  const auto result = greetWithHello("World");
  ASSERT_EQ(result, "Hello World!");
}

TEST(ReadmeExamples, pipe_with_at_simple) {
  auto incrementFn = [](int value) { return value + 1; };

  auto pipe = makePipe(at<1>(incrementFn), at<int>(incrementFn));

  const auto result = pipe(1.0, 2);
  ASSERT_EQ(result, std::make_tuple(1.0, 4));
}

TEST(ReadmeExamples, pipe_with_at_advanced) {
  auto provideNameAndYearOfBirth = []() { return std::make_tuple("Haskell Curry"s, 1900); };
  auto toBirthString = [](auto year) { return "born in "s + std::to_string(year); };
  struct Separator {
    std::string characters_;
  };
  auto concat = [](const std::string& lhs, const std::string& rhs, Separator separator) {
    return lhs + separator.characters_ + rhs;
  };

  auto providePersonInfoByType =
      makePipe(at<>(provideNameAndYearOfBirth), at<int>(toBirthString), at<std::string, Separator>(concat));

  auto providePersonInfoByIndex = makePipe(at<>(provideNameAndYearOfBirth), at<2>(toBirthString), at<1, 2, 0>(concat));

  ASSERT_EQ(providePersonInfoByType(Separator{", "}), "Haskell Curry, born in 1900"s);
  ASSERT_EQ(providePersonInfoByIndex(Separator{" | "}), "Haskell Curry | born in 1900"s);
}

TEST(ReadmeExamples, make_callable) {
  class Appender {
    std::string appendix_;

   public:
    Appender(std::string appendix) : appendix_{appendix} {}
    std::string append(std::string arg) const { return arg + appendix_; }
  };

  Appender appender{"A"};
  auto pipe = makePipe(MAKE_CALLABLE(std::to_string), MAKE_CALLABLE(appender.append));

  ASSERT_EQ(pipe(0), "0A");
}

TEST(ReadmeExamples, pass_along_by_index) {
  auto plusFn = [](int lhs, int rhs) { return lhs + rhs; };
  auto multiplyFn = [](int lhs, int rhs) { return lhs * rhs; };

  auto pipe = makePipe(passAlong<1>(plusFn), multiplyFn);

  ASSERT_EQ(pipe(1, 2), 6);
}

TEST(ReadmeExamples, pass_along_by_type) {
  enum class Locale { en_US, de_DE };
  auto appendDateFn = [](std::string buffer, Locale config) {
    buffer += (config == Locale::en_US) ? "9/15/1959"s : "15.09.1959"s;
    return buffer;
  };
  auto appendSpaceFn = [](std::string buffer, Locale) {
    buffer += " ";
    return buffer;
  };

  auto appendTimeFn = [](std::string buffer, Locale config) {
    buffer += (config == Locale::en_US) ? "12:01 AM"s : "00:01"s;
    return buffer;
  };

  auto appendDateTime = makePipe(passAlong<Locale>(appendDateFn), passAlong<Locale>(appendSpaceFn), appendTimeFn);

  ASSERT_EQ(appendDateTime("en_US: "s, Locale::en_US), "en_US: 9/15/1959 12:01 AM"s);
  ASSERT_EQ(appendDateTime("de_DE: "s, Locale::de_DE), "de_DE: 15.09.1959 00:01"s);
}
