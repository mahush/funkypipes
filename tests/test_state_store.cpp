//
// Copyright (c) 2024 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <tuple>

#include "funkypipes/state_store.hpp"
#include "utils/move_only_struct.hpp"

using namespace funkypipes;
using ::testing::MockFunction;

//
// `constructor` and `get_state` Method related tests
//

// Ensure that a default constructed store is initialized with default constructed state
TEST(StateStoreTest, DefaultState) {
  // When: A store initialized with an default state
  StateStore<int> store;

  // Then: The default state is active
  EXPECT_EQ(store.get_state(), 0);
}

// Ensure that an initial state can be set
TEST(StateStoreTest, InitialStateAndSubscription) {
  // When: A store initialized with a given initial state and a subscription
  const int initialState = 10;
  auto dummySubscriptionFn = [](const std::string&, const int&, const int&) {};
  StateStore<int> store(dummySubscriptionFn, initialState);

  // Then: The given initial state is active
  EXPECT_EQ(store.get_state(), initialState);
}

// Ensure that an initial state can be set
TEST(StateStoreTest, InitialStateOnly) {
  // When: A store initialized with a given initial state
  const int initialState = 10;
  StateStore<int> store(initialState);

  // Then: The given initial state is active
  EXPECT_EQ(store.get_state(), initialState);
}

//
// `apply` Method related tests
//

// Ensure that state is updated by [state->state] update function
TEST(StateStoreTest, ApplyingUpdateFunctionThatReturnsStateOnlyAffectsState) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function modifies the state
  store.apply("increment", [](int state) { return ++state; });

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 1);
}

// Ensure that state is updated by [state,input->state] update function
TEST(StateStoreTest, ApplyingUpdateFunctionThatTakesInputAffectsState) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function taking input modifies the state
  auto updateFn = [](int state, int amount) { return state + amount; };
  const auto input = 2;
  store.apply("increment", updateFn, input);

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 2);
}

// Ensure that state is updated by [state->state,output] update function
TEST(StateStoreTest, ApplyingUpdateFunctionThatReturnsStateAndOutputAffectsState) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function modifies the state
  auto updateFn = [](int state) { return std::make_tuple(++state, "output"); };
  std::ignore = store.apply("increment", updateFn);

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 1);
}

// Ensure that output is returned when applying [state->state,output] update function
TEST(StateStoreTest, ApplyReturnsSingleOutput) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function returns a tuple with changed state and output
  auto updateFn = [](int state) {
    ++state;
    const auto output = state * 2;
    return std::make_tuple(state, output);
  };
  const auto result = store.apply("incrementAndGetDouble", updateFn);

  // Then: The output is returned directly
  EXPECT_EQ(result, 2);
}

// Ensure that move only output is returned when applying [state->state,output] update function
TEST(StateStoreTest, ApplyReturnsMoveOnlyOutput) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function returns a tuple with state change and move only output
  auto updateFn = [](int state) {
    ++state;
    return std::make_tuple(state, MoveOnlyStruct{state});
  };
  const auto result = store.apply("incrementAndGetWrappedInMoveOnly", updateFn);

  // Then: The output is returned without copying
  EXPECT_EQ(result.value_, 1);
}

// Ensure that output1 and output2 is returned when applying [state->state,output1,output2] update function
TEST(StateStoreTest, ApplyReturnsMultipleOutputs) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function returns a tuple with state change and additional output
  const auto result = store.apply("incrementAndGetStringAndDouble", [](int state) {
    ++state;
    const auto output1 = std::to_string(state);
    const auto output2 = state * 2;
    return std::make_tuple(state, output1, output2);
  });

  // Then: The outputs is returned as tuple
  EXPECT_EQ(result, std::make_tuple(std::string{"1"}, 2));
}

// Ensure that a specified subscription is called with correct arguments
TEST(StateStoreTest, ApplyCallsSubscriptionFunction) {
  // Given: A store initialized with subscription
  MockFunction<void(const std::string&, const int&, const int&)> subscriptionFn;
  StateStore<int> store(subscriptionFn.AsStdFunction());

  // Then: Subscription called with correct parameters
  EXPECT_CALL(subscriptionFn, Call("increment", 0, 1));

  // When: Update function is applied
  auto updateFn = [](int state) { return ++state; };
  store.apply("increment", updateFn);
}

//
// `applyAndTransform` Method related tests
//

// Ensure that updating state based on [state->state] update function works
TEST(StateStoreTest, ApplyAndTransformUpdatesState) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function modifies the state
  auto updateFn = [](int state) { return ++state; };
  auto transformFn = [](int state) { return std::to_string(state); };
  std::ignore = store.applyAndTransform("incrementAndReturnString", updateFn, transformFn);

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 1);
}

// Ensure that transforming state based on [state->state] update function works
TEST(StateStoreTest, ApplyAndTransformReturnsBasedOnStateOnly) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function modifies the state and a transformation function transforms it
  auto updateFn = [](int state) { return ++state; };
  auto transformFn = [](int state) { return std::to_string(state); };
  auto result = store.applyAndTransform("incrementAndReturnString", updateFn, transformFn);

  // Then: The transformed result is returned
  EXPECT_EQ(result, "1");
}

// Ensure that transforming state based on [state->state,output] update function works
TEST(StateStoreTest, ApplyAndTransformReturnsBasedOnSingleOutput) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function provides state + output to a transformation function
  auto updateFn = [](int state) { return std::make_tuple(++state, "output"); };
  auto transformFn = [](int state, const char* output) { return std::to_string(state) + "," + output; };
  auto result = store.applyAndTransform("incrementAndReturnString", updateFn, transformFn);

  // Then: The transformation got applied
  EXPECT_EQ(result, "1,output");
}

// Ensure that transforming state based on [state->state,output1, output2] update function works
TEST(StateStoreTest, ApplyAndTransformReturnsBasedOnMultipleOutput) {
  // Given: A default initialized store
  StateStore<int> store;

  // When: An update function provides state + output1 + output2 to a transformation function
  auto updateFn = [](int state) { return std::make_tuple(++state, "output1", "output2"); };
  auto transformFn = [](int state, const char* output1, const char* output2) {
    return std::to_string(state) + "," + output1 + "," + output2;
  };
  auto result = store.applyAndTransform("incrementAndReturnString", updateFn, transformFn);

  // Then: The transformation got applied
  EXPECT_EQ(result, "1,output1,output2");
}

// Ensure that a specified subscription is called with correct arguments
TEST(StateStoreTest, ApplyAndTransformCallsSubscriptionFunction) {
  // Given: A store initialized with subscription
  MockFunction<void(const std::string&, const int&, const int&)> subscriptionFn;
  StateStore<int> store(subscriptionFn.AsStdFunction());

  // Then: Subscription called with correct parameters
  EXPECT_CALL(subscriptionFn, Call("incrementAndReturnString", 0, 1));

  // When: Update function is applied
  auto updateFn = [](int state) { return ++state; };
  auto transformFn = [](int state) { return std::to_string(state); };
  auto result = store.applyAndTransform("incrementAndReturnString", updateFn, transformFn);
}

//
// `bind` Method related tests
//

// Ensure that updating state based on [state->state] update function works
TEST(StateStoreTest, BindUpdatesState) {
  // Given: A lambda which bound a update function to a store
  StateStore<int> store;
  auto updateFn = [](int state) { return ++state; };
  auto boundUpdateFn = store.bind("increment", updateFn);

  // When: The lambda is called
  boundUpdateFn();

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 1);
}

// Ensure that output provided by [state->state, output] update function is returned
TEST(StateStoreTest, BindUpdateProvidingOutputIsReturned) {
  // Given: A lambda which bound a update function to a store
  StateStore<int> store;
  auto updateFn = [](int state) { return std::make_tuple(++state, "output"); };
  auto boundUpdateFn = store.bind("incrementAndReturnOutput", updateFn);

  // When: The lambda is called
  const auto output = boundUpdateFn();

  // Then: The output is returned
  EXPECT_EQ(output, "output");
}

// Ensure that transformation result based on [state->state] update function is returned
TEST(StateStoreTest, BindUpdateAndTransformResultReturned) {
  // Given: A lambda which bound an update and a transform function to a store
  StateStore<int> store;
  auto updateFn = [](int state) { return ++state; };
  auto transformFn = [](int state) { return std::to_string(state); };
  auto boundUpdateAndTransformFn = store.bind("incrementAndReturnString", updateFn, transformFn);

  // When: The labmda is called
  const auto result = boundUpdateAndTransformFn();

  // Then: The transformation result is returned
  EXPECT_EQ(result, "1");
}

// Ensure that passing the update function as rvalue works
TEST(StateStoreTest, BindUpdateFunctionAsRValue) {
  // Given: A lambda which bound a update function as rvalue to a store
  StateStore<int> store;
  auto boundUpdateFn = store.bind("increment", [](int& state) { return ++state; });

  // When: The labmda is called
  boundUpdateFn();

  // Then: The store's state is updated
  EXPECT_EQ(store.get_state(), 1);
}

// Ensure that a specified subscription is called with correct arguments
TEST(StateStoreTest, BindCallsSubscriptionFunction) {
  // Given: A update function bound to a store that has a subscription
  MockFunction<void(const std::string&, const int&, const int&)> subscriptionFn;
  StateStore<int> store(subscriptionFn.AsStdFunction());
  auto updateFn = [](int state) { return ++state; };
  auto boundUpdateFn = store.bind("increment", updateFn);

  // Then: Subscription called with correct parameters
  EXPECT_CALL(subscriptionFn, Call("increment", 0, 1));

  // When: Update function is applied
  boundUpdateFn();
}

