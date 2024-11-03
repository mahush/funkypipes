
# Roadmap

The following higher-order functions are planned to be implemented in order to extend the range of tools provided by this library.


1. **bindLeft**
    - **Type Signature**: `(a1 -> a2 -> ... -> an -> b) -> (a1 -> a2 -> ... -> ak) -> (ak+1 -> ... -> an -> b)`
    - **Description**: Creates a new function by pre-filling the first argument of a function.
    - **See Also**: `bind_left`(c++20),`partial`(ramda or lodash)
    - **Usage Example**:
      ```cpp
      auto add = [](int x, int y) { return x + y; };

      auto addFive = bindLeft(add, 5);
      addFive(3);  // returns 8
      ```

1. **cond**
   - **Type Signature**: `[(a -> Bool, a -> b)] -> (a -> b)`
   - **Description**: Evaluates a list of predicate-function pairs and applies the function corresponding to the first predicate that returns true.
   - **See Also**: `cond`(ramda or lodash)
   - **Usage Example**:
     ```cpp
     auto isZero = [](int x) { return x == 0; };
     auto isPositive = [](int x) { return x > 0; };
     auto toString = [](int x) { return std::to_string(x); };

     auto caseFunction = cond({{isZero, [](int) { return "zero"; }},
                               {isPositive, toString},
                               {[](int) { return true; }, [](int) { return "negative"; }}});

     caseFunction(0);  // returns "zero"
     caseFunction(5);  // returns "5"
     caseFunction(-5); // returns "negative"
     ```

1. **constant**
    - **Type Signature**: `a -> (b -> a)`
    - **Description**: Creates a function that always returns the same value.
    - **See Also**: `always` (ramda), `constant` (lodash)
    - **Usage Example**:
      ```cpp
      auto alwaysFive = constant(5);

      alwaysFive(10);  // returns 5
      ```

1. **converge**
    - **Type Signature**: `(x1 -> x2 -> ... -> xn -> y) -> [a -> x1, a -> x2, ..., a -> xn] -> (a -> y)`
    - **Description**: Applies multiple functions to the same input and then combines the results using another function.
    - **See Also**: `converge` (ramda)
    - **Usage Example**:
      ```cpp
      auto sum = [](int x, int y) { return x + y; };
      auto doubleIt = [](int x) { return x * 2; };
      auto tripleIt = [](int x) { return x * 3; };

      auto convergeFunction = converge(sum, doubleIt, tripleIt);
      convergeFunction(5);  // returns 25
      ```

1. **filterUnchanged**
    - **Type Signature**: `a -> a -> std::optional<a>`
    - **Description**: Returns an `std::optional` containing its argument if it has changed since the last call; otherwise, returns `std::nullopt`.
    - **Usage Example**:
      ```cpp
      auto filterUnchanged = [](int newValue) {
          static std::optional<int> lastValue;
          if (!lastValue || *lastValue != newValue) {
              lastValue = newValue;
              return newValue;
          }
          return std::nullopt;
      };

      filterUnchanged(5);  // returns std::optional<int>(5)
      filterUnchanged(5);  // returns std::nullopt
      filterUnchanged(10); // returns std::optional<int>(10)
      ```

1. **identity**
    - **Type Signature**: `a -> a`
    - **Description**: A function that returns its argument unchanged.
    - **See Also**: `identity` (ramda or lodash)
    - **Usage Example**:
      ```cpp
      auto identityFunction = identity<int>;

      identityFunction(5);  // returns 5
      ```

1. **ifElse**
    - **Type Signature**: `(a -> Bool) -> (a -> b) -> (a -> b) -> a -> b`
    - **Description**: Conditionally applies one of two functions based on a predicate.
    - **See Also**: `ifElse` (ramda)
    - **Usage Example**:
      ```cpp
      auto isEven = [](int x) { return x % 2 == 0; };
      auto doubleIt = [](int x) { return x * 2; };
      auto tripleIt = [](int x) { return x * 3; };

      auto ifElseFunction = ifElse(isEven, doubleIt, tripleIt);
      ifElseFunction(4);  // returns 8
      ```

1. **memoize**
    - **Type Signature**: `(a -> b) -> (a -> b)`
    - **Description**: Caches the results of a function call based on its inputs to avoid redundant calculations.
    - **See Also**: `memoizeWith`(ramda), `memoize` (Lodash)
    - **Usage Example**:
      ```cpp
      auto slowFunction = [](int x) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return x * 2;
      };

      auto memoizedFunction = memoize(slowFunction);

      memoizedFunction(5);  // returns 10
      memoizedFunction(5);  // returns 10
      ```

1. **mergeByType / mergeTuples**
    - **Type Signature**: `(a -> b -> c) -> (a, d) -> (b, e) -> (c, d, e)`
    - **Description**: Merges elements of the same type from two tuples using type-specific functions.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double, std::string> tuple1 = std::make_tuple(1, 2.5, "3");
      std::tuple<int, double, std::string> tuple2 = std::make_tuple(4, 1.5, "4");

      auto intFunction = [](int x, int y) { return x + y; };
      auto doubleFunction = [](double x, double y) { return x * y; };
      auto stringFunction = [](std::string x, std::string y) { return x + y; };

      mergeByType(tuple1, tuple2, intFunction, doubleFunction, stringFunction);  // returns std::tuple<int, double, std::string>(5, 3.75, "34")
      ```

1. **on**
    - **Type Signature**: `(b -> b -> c) -> (a -> b) -> a -> a -> c`
    - **Description**: Applies a binary function to the results of applying a unary function to two arguments.
    - **See Also**: `on`(ramda)
    - **Usage Example**:
      ```cpp
      auto add = [](int x, int y) { return x + y; };
      auto square = [](int x) { return x * x; };

      auto onFunction = on(add, square);
      onFunction(3, 4);  // returns 25
      ```

1. **otherwise (orElse)**
    - **Type Signature**: `() -> b -> std::optional<a> -> b`
    - **Description**: A higher-order function that takes a fallback function (with no arguments) and returns a new function. The new function either forwards the value from the `std::optional` if it has one, or executes the fallback function and returns its result if the `std::optional` is empty.
    - **See Also**: `otherwise`(ramda)
    - **Usage Example**:
      ```cpp
      auto orElse = [](auto fallbackFunc) {
          return [=](std::optional<int> opt) {
              if (opt) {
                  return *opt;
              }
              return fallbackFunc();
          };
      };

      auto fallbackFunc = []() { return 20; };
      auto handleOptional = orElse(fallbackFunc);

      handleOptional(std::optional<int>(10));  // returns 10
      handleOptional(std::nullopt);            // returns 20
      ```

1. **retry**
    - **Type Signature**: `(a -> b) -> Int -> (a -> b)`
    - **Description**: Attempts a function multiple times on failure due to exceptions.
    - **Usage Example**:
      ```cpp
      auto unreliableFunction = [](int x) {
          static int attempt = 0;
          attempt++;
          if (attempt < 3) throw std::runtime_error("Failed");
          return x * 2;
      };

      const int maxAttempts = 10;
      auto retryFunction = retry(unreliableFunction, maxAttempts);
      retryFunction(5);  // returns std::optional<int>(10)
      ```

1. **retryOnNullopt**
    - **Type Signature**: `(a -> std::optional<b>) -> Int -> (a -> std::optional<b>)`
    - **Description**: Attempts a function multiple times if it returns nullopt.
    - **Usage Example**:
      ```cpp
      auto unreliableFunction = [](int x) -> std::optional<int> {
          static int attempt = 0;
          attempt++;
          if (attempt < 3) return std::nullopt;
          return x * 2;
      };

      const int maxAttempts = 10;
      auto retryOptionalFunction = retryOnNullopt(unreliableFunctionm maxAttempts);
      retryOptionalFunction(5);  // returns std::optional<int>(10)
      ```

1. **tap**
    - **Type Signature**: `(a -> b) -> a -> a`
    - **Description**: Performs a side effect and returns the original value.
    - **See Also**: `tap` (ramda), `tap`(lodash)
    - **Usage Example**:
      ```cpp
      auto logValue = [](int x) {
          std::cout << "Value: " << x << std::endl;
      };

      tap(logValue, 5);  // returns 5 and prints: "Value: 5"
      ```

1. **transformAt** (index)
    - **Type Signature**: `(a -> b) -> Int -> (a, c, d) -> (b, c, d)`
    - **Description**: Applies a function to the nth element in a tuple.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double, std::string> mixedTuple = std::make_tuple(1, 2.5, "3");

      auto transformFunction = [](int x) { return x * 10; };

      transformAt(mixedTuple, 0, transformFunction);  // returns std::tuple<int, double, std::string>{10, 2.5, "3"}
      ```

1. **transformAt** (type)
    - **Type Signature**: `(a -> b) -> (c, a, d) -> (c, b, d)`
    - **Description**: Selects elements of a specific type, applies a transformation, and then merges back into the original tuple structure.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double, std::string> mixedTuple = std::make_tuple(1, 2.5, "3");

      auto transformFunction = [](int x) { return x * 10; };

      transformAt<int>(mixedTuple, transformFunction);  // returns std::tuple<int, double, std::string>{10, 2.5, "3"}
      ```

1. **transformByType**
    - **Type Signature**: `[(a -> b), (c -> d), ...] -> (a, c, ...) -> (b, d, ...)`
    - **Description**: Applies different functions to elements based on their type, and returns a tuple of the results.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double, std::string> mixedTuple = std::make_tuple(1, 2.5, "3");

      auto intFunction = [](int x) { return x * 2; };
      auto doubleFunction = [](double x) { return x * 2.0; };
      auto stringFunction = [](std::string x) { return x + "!"; };

      transformByType(mixedTuple, intFunction, doubleFunction, stringFunction);  // returns std::tuple<int, double, std::string>{2, 5.0, "3!"}
      ```

1. **transformEach**
    - **Type Signature**: `(a -> b) -> (a, c, d) -> (b, c, d)`
    - **Description**: Applies the same function to each element in the tuple.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double, std::string> mixedTuple = std::make_tuple(1, 2.5, "3");

      auto transformFunction = [](auto x) { return std::to_string(x) + "_transformed"; };

      transformEach(transformFunction, mixedTuple);  // returns std::tuple<std::string, std::string, std::string>{"1_transformed", "2.5_transformed", "3_transformed"}
      ```

1. **transformHeterogeneous**
    - **Type Signature**: `[(a -> b)] -> a -> [b]`
    - **Description**: Applies multiple functions to a single input and returns a tuple of the results.
    - **See Also**: `juxt`(ramda), `ap`(ramda), `over`(lodash)
    - **Usage Example**:
      ```cpp
      auto increment = [](int x) { return x + 1; };
      auto decrement = [](int x) { return x - 1; };

      transformHeterogeneous({increment, decrement})(5);  // returns std::tuple{6, 4}
      ```

1. **tryOrElse**
    - **Type Signature**: `(a -> Maybe b) -> (a -> b) -> a -> b`
    - **Description**: Tries a primary function and uses an alternative if it fails.
    - **See Also**: `tryCatch`(ramda)
    - **Usage Example**:
      ```cpp
      auto primary = [](int x) -> std::optional<int> {
          if (x == 0) return std::nullopt;
          return x;
      };
      auto fallback = [](int x) { return x + 1; };

      tryOrElse(primary, fallback)(0);  // returns 1
      ```

1. **when / unless**
    - **Type Signature**: `(a -> Bool) -> (a -> a) -> a -> a`
    - **Description**: Applies a function only if a condition is met.
    - **See Also**: `when`(ramda), `unless`(ramda)
    - **Usage Example**:
      ```cpp
      auto isEven = [](int x) { return x % 2 == 0; };
      auto doubleIfEven = when(isEven, [](int x) { return x * 2; });

      doubleIfEven(3);  // returns 3
      doubleIfEven(4);  // returns 8
      ```
