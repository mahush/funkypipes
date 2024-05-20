![image](https://github.com/mahush/funkypipes/blob/main/docs/funkypipes_banner.png)

Your C++17-compatible higher-order function toolbox for building heterogeneous data transformation pipelines by composing functions in a tacit programming style.

## Features in general
This library helps in construction of data processing pipelines, out of a given series of callables, each designed to transform input into output. These callables can be linked together in a chain, by composing them into a single unified callable, a pipe. A pipe accepts the same argument as the first callable in its chain and return the output of the last callable in its chain. Below is a detailed list of features:

- **Language Standard**: c++17
- **Callable Types**: functions, lambdas, and functors are supported
- **Pipe Object**: The resulting pipe is technically a lambda, as such it is assignable to std::function. This also means that a pipe can be composed of other pipes.
- **Callable Signatures**: Callables having multiple parameters and tuple return values are supported. Furthermore const and non-const references for parameters and return values are supported. But there is no support for callables return void type.
- **Compile-Time Signature Validation**: In case of mismatching callable signatures, meaningful compile time error messages are provided.
- **Generic and Overloaded Callables**: Generic lambdas, generic functors and overloaded functors are supported as callables to be composed. This means a pipe might take different input argument types that might even result in different output types. Thus, the pipe's signature is as flexible as the signatures of its chained callables itself.
- **Chain Breaking**: Depended on the specific pipe creation tool, chain breaking is handled differently. Generally, a callable may return its result wrapped in std::optional. This allows a callable to break the chain by providing a `std::nullopt`. 
- **Error Handling**: The implementation is exception-safe. Exceptions thrown by any callable of in the pipe are propagated to the pipe's caller.
- **Move Semantics and Perfect Forwarding**: The implementation fully supports move semantics for callables and their arguments.

## Tools

- **makeFailablePipe**: A function template that creates a pipe out of the given callables. The pipe returns the result wrapped in
 std::optional. On success this are the return values of the last callable in the chain. On failure, triggered by any
 callable returning std::nullopt, it's a std::nullopt according to the return value type of the last callable in the
 chain.
- **More to come...**

## Examples

### makeFailablePipe

#### Basic example
```cpp
  auto boolToInt = [](bool flag) -> int { return static_cast<int>(flag); };
  auto toString = [](auto arg) -> std::string { return std::to_string(arg); };
  auto twoTimes = [](const std::string& string) -> std::string { return string + string; };

  auto pipe = makeFailablePipe(boolToInt, toString, twoTimes);

  std::optional<std::string> result = pipe(true);

  ASSERT_EQ(result, "11");
```

#### Multiple parameter example
```cpp
  auto generateSomeData = [](int arg1, int arg2, int arg3) -> std::tuple<int, std::string> {
    return {arg1 + arg2 + arg3, "4"};
  };
  auto mergeToString = [](int value, std::string string) { return std::to_string(value) + string; };

  auto pipe = makeFailablePipe(generateSomeData, mergeToString);

  std::optional<std::string> result = pipe(1, 2, 3);

  ASSERT_EQ(result, "64");
```

#### Chain breaking example
```cpp
  auto breakWhenZero = [](int value) -> std::optional<int> {
    return (value == 0) ? std::nullopt : std::make_optional(value);
  };
  auto forward = [](int value) -> int { return value; };

  auto pipe = makeFailablePipe(breakWhenZero, forward, forward);

  std::optional<int> res1 = pipe(0);  // breaking case
  EXPECT_FALSE(res1.has_value());

  std::optional<int> res2 = pipe(7);  // forwarding case
  EXPECT_EQ(res2, 7);
```

#### Generic example
```cpp
  auto forward = [](auto... args) { return std::make_tuple(args...); };
  auto sum = [](auto... args) { return (args + ...); };

  auto pipe = makeFailablePipe(forward, sum);

  ASSERT_EQ(pipe(0), 0);
  ASSERT_EQ(pipe(1, 2, 3), 6);
  ASSERT_EQ(pipe("1"s, "2"s, "3"s), "123"s);
```

#### Reference example
```cpp
  auto lambda = [](int& value) -> int& { return value; };

  auto pipe = makeFailablePipe(lambda, lambda);

  int argument{1};
  std::optional<std::reference_wrapper<int>> result = pipe(argument);
  ASSERT_EQ(result, 1);

  ASSERT_TRUE(result.has_value());
  result.value()++;
  ASSERT_EQ(argument, 2);
```

#### Recursive example
```cpp
  auto forward = [](bool flag) { return flag; };

  auto pipe1 = makeFailablePipe(forward, forward, forward);
  auto pipe2 = makeFailablePipe(pipe1, pipe1, forward);
  auto pipe3 = makeFailablePipe(pipe2, pipe2);

  ASSERT_EQ(pipe3(true), true);
```

