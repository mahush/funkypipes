![image](https://github.com/mahush/funkypipes/blob/main/docs/funkypipes_banner.png)

**Your C++17-compatible higher-order function toolbox for building heterogeneous data transformation pipelines by composing functions in a tacit programming style.**

- **Higher-order function toolbox**: This library provides a collection of functions (here called tools) that take other functions as arguments and return functions as result. These tools can be combined in various ways.
- **Heterogeneous data transformation pipelines**: The library enables building pipelines that transform multiple arguments of different types into results of different types based on user-provided callables that themselves transform multiple input types into multiple output types.
- **Callables**: Refers to functions, function objects, lambdas also overloaded and generic ones.
- **Composing in a tacit style**: Callables are composed into pipelines using the tools provided by the library without explicitly specifying the arguments. This style of programming is also known as point-free style, where the focus is on the composition of functions rather than the data they operate on.
---
## General Features

- **Language Standard**: c++17
- **Callable Types**: functions, lambdas, and functors are supported
- **Generic and Overloaded Callables**: Generic lambdas, generic functors and overloaded functors are supported as callables to be composed. This means a pipe might take different input argument types that might even result in different output types. Thus, the pipe's signature is as flexible as the signatures of its chained callables itself.
- **Error Handling**: The implementation is exception-safe. Exceptions thrown by any callable of in the pipe are propagated to the pipe's caller.
- **Callable Signatures**: Callables having multiple parameters and tuple return values are supported. In general `const` and non-`const` lvalue references and rvalue references for arguments and return values are supported.
- **Move Semantics and Perfect Forwarding**: The implementation fully supports move semantics for callables and their arguments where semantically meaningful.
---
## Tools and their specific features

### **makePipe**
A higher-order-function that links a given series of callables together in a chain, by composing them into a single unified callable, a pipe. The output of each callable is passed as input to the next callable in the chain.
  - **Pipe Object**: The resulting pipe is technically a lambda, as such it is assignable to std::function. This also means that a pipe can be composed of other pipes.
  - **Pipe Input**: A pipe accepts the same arguments as the first callable in its chain.
  - **Pipe Output**: The pipe's return type is the return type of the last given callable.
  - **Compile-Time Signature Validation**: In case of mismatching callable signatures, meaningful compile time error messages are provided.

Basic Example:
```cpp
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
```

Reference Example:
```cpp
auto forwardReference = [](bool& value) -> bool& { return value; };

auto pipe = makePipe(forwardReference, forwardReference);

bool argument{true};
bool& result = pipe(argument);
ASSERT_EQ(result, true);

result = false;
ASSERT_EQ(argument, false);
```

Nested Example:
```cpp
auto increment = [](int value) {
  ++value;
  return value;
};

auto pipe1 = makePipe(increment, increment);
auto pipe2 = makePipe(pipe1, increment);
auto pipe3 = makePipe(pipe2, pipe2);

ASSERT_EQ(pipe3(0), 6);
```

### **andThen**
A decorator for handling chain-breaking in pipes created with `makePipe`. Callables following a chain-breaking callable (which may return `std::optional`) must be wrapped with `andThen`. If `andThen` encounters a `std::nullopt`, the decorated callable is skipped.  
  - **Decorator Input**: The input must be a `std::optional`. If it has a value, it is forwarded to the decorated callable.
  - **Decorator Output**: If the callable executes, its result is wrapped in `std::optional`; otherwise, a `std::nullopt` is returned.

Consequently when used in conjunction with `makePipe`, if a callable in the chain can return a `std::optional`, the pipe's return type will be a `std::optional` containing the result of the last callable, or `std::nullopt` if the chain is broken.

Chain Breaking Example:
```cpp
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
```

### **makeAutoPipe**
Similar to `makePipe`, this function template links callables into a single unified callable or pipe. However, unlike `makePipe`, it introduces automatic chain-breaking behavior, similar to `andThen`, but without requiring explicit wrapping.

  - **Pipe Object**: See `makePipe`.
  - **Pipe Input**: See `makePipe`.
  - **Pipe Output**: The return type is either the last callable’s return type or, if any preceding callable returns a `std::optional`, the last callable’s return type wrapped in `std::optional`.
  - **Compile-Time Signature Validation**: See `makePipe`.
  - **Auto Aspect: Chain Breaking**: Unlike `makePipe`, `makeAutoPipe` allows any callable to return a `std::optional` to break the chain without explicit wrapping via `andThen`. If a `std::nullopt` is encountered, subsequent callables are skipped automatically, and `std::optional` values are unwrapped and forwarded to the next callable seamlessly.

Chain Breaking Example:
```cpp
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
```

### **bindFront**
Decorates the given function by pre-binding specified arguments from left to right, resulting in a new function that requires only the remaining arguments.

  - **Type Signature**: `(a1 -> a2 -> ... -> an -> b) -> (a1 -> a2 -> ... -> ak) -> (ak+1 -> ... -> an -> b)`
  - **See Also**: `bind_front`(c++20), `partial`(ramda or lodash)

Example:
```cpp
auto greet = [](const std::string& salutation, const std::string& name) { return salutation + " " + name + "!"; };

auto greetWithHello = bindFront(greet, "Hello");

const auto result = greetWithHello("World");
ASSERT_EQ(result, "Hello World!");
```

### **at**

A decorator for transforming only selected arguments. It takes a function and a list of indices/types, returning a new function that applies the original function to the specified arguments while leaving others unchanged. This allows selective transformation of arguments within a pipe.

  - **Input**: Out of any number of variadic arguments, only the ones at the specified indices/types are passed to the function.
  - **Output**: The concatenation of the untouched arguments and the results of the function applied to the selected arguments is returned as tuple. In case the tuple would only contain a single element, the element itself is returned.

  - **Type Signature**: `((a1, a2, ..., an) -> (b1, b2, ..., bn)) -> [Int] -> ((c1, c2, ..., cn) -> (c1, c2, ..., b1, b2, ...))`

      <details>
      <summary> Type signature details </summary>

      - **`(a1, a2, ..., an -> b1, b2, ..., bn)`**:
        - Represents the function to be applied.
        - `a1, a2, ..., an` are the types of the inputs to this function.
        - `b1, b2, ..., bn` are the types of the results from this function, emphasizing that it can take multiple inputs and produce multiple outputs.
      
      - **`[Int]`**:
        - A list of indices specifying which arguments from the tuple `(c1, c2, ..., cn)` should be transformed by the function.
        - These indices correspond to the positions in the tuple where the function `a -> b` is applied.
      
      - **`(c1, c2, ..., cn)`**:
        - The tuple of arguments provided to the resulting function.
        - `ci` represents each argument type, with some of these being transformed based on the indices.
      
      - **`(c1, c2, ..., b1, b2, ...)`**:
        - The output tuple of the resulting function.
        - Elements at the specified indices are transformed to `b1, b2, ..., bn` by applying the function, while other elements (`ci`) remain unchanged.

      </details>

Simple Example:
```cpp
auto incrementFn = [](int value) { return value + 1; };

auto pipe = makePipe(at<1>(incrementFn), at<int>(incrementFn));

const auto result = pipe(1.0, 2);
ASSERT_EQ(result, std::make_tuple(1.0, 4));
```

Advanced Example:
```cpp
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
```

### **makeCallable**

A macro that wraps the specified invocable expression into a lambda function. The resulting lambda can be invoked with
any number of arguments, which are then perfectly forwarded to the original invocable expression. The lambda returns
the result of this invocation.

This is especially useful for wrapping an overloaded function into a generic callable object, allowing it to be used as a pipeline element. Additionally, wrapping member function calls with this approach enhances readability compared to using `std::bind` or `std::bind_front`.

Example:
```cpp
// wrapping overloaded method
auto callable1 = MAKE_CALLABLE(std::to_string);
ASSERT_EQ(callable1(0), "0");
ASSERT_EQ(callable1(1.0), "1.000000");

// wrapping member function call
struct Foo {
  int bar(int arg) const { return arg; }
};
Foo foo;
auto callable2 = MAKE_CALLABLE(foo.bar);
ASSERT_EQ(callable2(3), 3);
```

### **passAlong**

A decorator function that duplicates the specified input arguments and passes the duplicates along. The decorated function processes all input arguments as usual. Finally the decorator function returns both the duplicated arguments and the output of the decorated function.

  - **Input**: All input arguments are forwarded to the decorated function.
  - **passing along**: The arguments specified as template parameter are duplicated and passed along.
  - **Output**: The concatenation of the duplicated input arguments and the result of the decorated function, in that exact order.


By Index Example:
```cpp
auto plusFn = [](int lhs, int rhs) { return lhs + rhs; };
auto multiplyFn = [](int lhs, int rhs) { return lhs * rhs; };

auto pipe = makePipe(passAlong<1>(plusFn), multiplyFn);

ASSERT_EQ(pipe(1, 2), 6);
```

By Type Example:
```cpp
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

```

### **more to come**
See the [Roadmap](https://github.com/mahush/funkypipes/blob/main/docs/roadmap.md)

---
## Project Directory Structure

```
/funkypipes
│
├── /examples                        # Example files
├── /includes                        # Header files
│   ├── /details                     # Internal header files
├── /test                            # Test files
│   ├── /predefined                  # Predefined tests
│   │   ├── /callable_type           # Predefined callable Type tests
│   │   ├── /signature_propagation   # Predefined signature propagation tests
│   │   └── /execution_semantics     # Predefined execution semantics tests
│   ├── /utils                       # Utility files
│   └── /module_specific             # Module-Specific tests
```
**Examples**: Usage examples of this lib's features implemented as gtest tests.

**Header Files**: The header files of this lib.

  - **Internal Header Files**:
  Library internal header files. Via the `details` subfolder and the `details` namespace the internal implementation details are separated from the public API.

**Test Files**:
  This folder contains the tests based on the gtest framework.

   - **Predefined Tests**:
   These tests focus on standardized criteria for decorators and higher-order functions. These tests can be reused across different modules.

       - **Predefined Callable Type Tests**: Ensures support for various callable types like functions, lambdas, callable objects or special cases like move-only callables.

       - **Predefined Signature Propagation Tests**: Verifies that the function’s signature (arguments and return values) is correctly preserved through decorators or higher-order functions.

       - **Predefined Execution Semantics Tests**: Checks the functional behavior of decorators and higher-order-functions.

  - **Utility Files**:
    Files that provide code that supports testing.

  - **Module-Specific Tests**:
     These tests are tailored to individual modules and can utilize predefined tests where applicable. They ensure that the unique functionality of specific compilation units is thoroughly validated.


