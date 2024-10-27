![image](https://github.com/mahush/funkypipes/blob/main/docs/funkypipes_banner.png)

Your C++17-compatible higher-order function toolbox for building heterogeneous data transformation pipelines by composing functions in a tacit programming style.

## General Features
This library helps in construction of data processing pipelines, out of a given series of callables, each designed to transform input into output. Below is a detailed list of general features:

- **Language Standard**: c++17
- **Callable Types**: functions, lambdas, and functors are supported
- **Callable Signatures**: Callables having multiple parameters and tuple return values are supported. Furthermore const and non-const references for parameters and return values are supported.
- **Compile-Time Signature Validation**: In case of mismatching callable signatures, meaningful compile time error messages are provided.
- **Generic and Overloaded Callables**: Generic lambdas, generic functors and overloaded functors are supported as callables to be composed. This means a pipe might take different input argument types that might even result in different output types. Thus, the pipe's signature is as flexible as the signatures of its chained callables itself.
- **Error Handling**: The implementation is exception-safe. Exceptions thrown by any callable of in the pipe are propagated to the pipe's caller.
- **Move Semantics and Perfect Forwarding**: The implementation fully supports move semantics for callables and their arguments.

## Tools and their specific features

- **makeAutoPipe**: A function template that links the given callables together in a chain, by composing them into a single unified callable, a pipe. 
  - Pipe Object: The resulting pipe is technically a lambda, as such it is assignable to std::function. This also means that a pipe can be composed of other pipes.
  - Pipe Input: A pipe accepts the same argument as the first callable in its chain.
  - Pipe Result: The pipe's return type is the return type of the last given callable, unless any preceding callable in the chain returns a `std::optional`. In that case, the return type is the last callable's return type wrapped in a `std::optional`.
  - Chain Breaking: Any callable may return a `std::optional` to be able to break the chain. No callable should take a `std::optional` as input because if a `std::nullopt` is encountered, it will not be forwarded to the callable; instead, the callable will be skipped.
  - Auto Aspect: If a preceding callable provides a `std::optional` with a value, it is automatically extracted before being forwarded to the subsequent callable. If a preceding callable provides a `std::nullopt`, all subsequent callables are skipped.

- **More to come...**

## Examples

### makeAutoPipe

#### Basic example
```cpp
  auto boolToInt = [](bool flag) -> int { return static_cast<int>(flag); };
  auto toString = [](auto arg) -> std::string { return std::to_string(arg); };
  auto twoTimes = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makeAutoPipe(boolToInt, toString, twoTimes);

  std::string result = pipe(true);

  ASSERT_EQ(result, "11");
```

#### Multiple parameter example
```cpp
  auto generateSomeData = [](int arg1, int arg2, int arg3) -> std::tuple<int, std::string> {
    return {arg1 + arg2 + arg3, "4"};
  };
  auto mergeToString = [](int value, std::string string) { return std::to_string(value) + string; };

  auto pipe = makeAutoPipe(generateSomeData, mergeToString);

  std::string result = pipe(1, 2, 3);

  ASSERT_EQ(result, "64");
```

#### Chain breaking example
```cpp
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };
  auto forward = [](int value) -> int { return value; };

  auto pipe = makeAutoPipe(breakWhenZero, forward, forward);

  std::optional<int> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<int> res2 = pipe(7);  // forwarding case
  EXPECT_EQ(res2, 7);
```

#### Generic example
```cpp
  auto forward = [](auto... args) { return std::make_tuple(args...); };
  auto sum = [](auto... args) { return (args + ...); };

  auto pipe = makeAutoPipe(forward, sum);

  ASSERT_EQ(pipe(0), 0);
  ASSERT_EQ(pipe(1, 2, 3), 6);
  ASSERT_EQ(pipe("1"s, "2"s, "3"s), "123"s);
```

#### Reference example
```cpp
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeAutoPipe(lambda, lambda);

  int argument{1};
  int& result = pipe(argument);
  ASSERT_EQ(result, 1);

  result++;
  ASSERT_EQ(argument, 2);
```

#### Nested example
```cpp
  auto increment = [](int value) {
    ++value;
    return value;
  };

  auto pipe1 = makeAutoPipe(increment, increment, increment);
  auto pipe2 = makeAutoPipe(pipe1, pipe1, increment);
  auto pipe3 = makeAutoPipe(pipe2, pipe2);

  ASSERT_EQ(pipe3(0), 14);
```

