
# Roadmap

The following higher-order functions are planned to be implemented in order to extend the range of tools provided by this library.


1. **constant**
    - **Type Signature**: `a -> (b -> a)`
    - **Description**: Creates a function that always returns the same value.
    - **See Also**: `always` (ramda), `constant` (lodash)
    - **Usage Example**:
      ```cpp
      auto alwaysFive = constant(5);

      alwaysFive(10);  // returns 5
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

1. **fork**
    - **Type Signature**: `[(a -> b)] -> a -> [b]`
    - **Description**: Applies multiple functions to a single input and returns a tuple of the results.
    - **See Also**: `juxt`(ramda), `ap`(ramda), `over`(lodash)
    - **Usage Example**:
      ```cpp
      auto increment = [](int x) { return x + 1; };
      auto decrement = [](int x) { return x - 1; };

      fork({increment, decrement})(5);  // returns std::make_tuple(6, 4)
      ```

1. **each**
    - **Type Signature**: `(a -> b) -> (a, c, d) -> (b, c, d)`
    - **Description**: Applies the same function to each element in the tuple.
    - **Usage Example**:
      ```cpp
      std::tuple<int, double> mixedTuple = std::make_tuple(1, 2.5);

      auto transformFunction = [](auto x) { return std::to_string(x) + "!"; };

      each(transformFunction)(mixedTuple);  // returns std::make_tuple("1!", "2.5!")
      ```

1. **when (unless)**
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

1. **ifElse**
    - **Type Signature**: `(a -> Bool) -> (a -> b) -> (a -> b) -> a -> b`
    - **Description**: Conditionally applies one of two functions based on a predicate.
    - **See Also**: `ifElse` (ramda)
    - **Usage Example**:
      ```cpp
      auto isEven = [](int x) { return x % 2 == 0; };
      auto doubleIt = [](int x) { return x * 2; };
      auto tripleIt = [](int x) { return x * 3; };

      auto doubleOrTriple = ifElse(isEven, doubleIt, tripleIt);
      doubleOrTriple(2);  // returns 4
      doubleOrTriple(3);  // returns 9
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

      orElse(fallbackFunc)(std::optional<int>(10));  // returns 10
      orElse(fallbackFunc)(std::nullopt);            // returns 20
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

      const int maxAttempts = 3;
      retryOnNullopt(unreliableFunctionm maxAttempts)(5);  // returns std::optional<int>(10)
      ```

1. **retryOnException**
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

      const int maxAttempts = 3;
      retry(unreliableFunction, maxAttempts)(5);  // returns std::optional<int>(10)
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

      tap(logValue)(5);  // returns 5 and prints: "Value: 5"
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
