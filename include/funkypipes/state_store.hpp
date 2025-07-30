// Copyright (c) 2025 mahush (info@mahush.de)
//
// Distributed under MIT License
//
// Official repository: https://github.com/mahush/funkypipes
//

#ifndef FUNKYPIPES_STATE_STORE_HPP
#define FUNKYPIPES_STATE_STORE_HPP

#include <functional>
#include <string>

#include "funkypipes/details/tuple/collapse_or_forward_tuple.hpp"
#include "funkypipes/details/tuple/separate_tuple_elements.hpp"
#include "funkypipes/details/tuple/tuple_traits.hpp"
#include "funkypipes/details/with_tuple_arg_unpacked.hpp"

namespace funkypipes {

// StateStore is a generic class template for managing and updating a state value of type TState. It provides flexible
// interfaces for applying and binding state update functions (TStateUpdateFn), supports optional transformation of
// update results (TTransformFn), and allows external subscribers to observe state transitions (SubscriptionFn).
//
// State update functions can be applied directly using apply, which immediately updates the state and returns any
// result. Alternatively, bind returns a callable with the update function (and optionally a transformation function)
// bound to the store instance, allowing you to perform state updates on that particular store simply by calling the
// resulting function.
//
// Expected Signatures:
//
// TStateUpdateFn:
//   - Accepts the current state (TState, TState& oldState, const TStateference) and any number of additional arguments.
//   - Returns either a new state (TState) or a tuple where the first element is the new state and the remaining
//     elements are additional outputs.
//   - Example signatures:
//       TState updateFn(TState state, Args... args);
//       std::tuple<TState, Output...> updateFn(TState state, Args... args);
//
// TTransformFn:
//   - Accepts the outputs returned by the update function (state and any additional outputs).
//   - Returns a transformed result of any type.
//   - Example signature:
//       ResultType transformFn(TState, Output...);
//
// SubscriptionFn:
//   - Accepts the update name, the previous state, and the new state.
//   - Example signature:
//       void subscriptionFn(const std::string& updateName, const TState& oldState, const TState& newState);
//
template <typename TState>
class StateStore {
 public:
  using UpdateName = std::string;
  using SubscriptionFn = std::function<void(UpdateName, const TState&, const TState&)>;

  // Constructor: Initializes the store with an optional subscription callback and initial state.
  explicit StateStore(SubscriptionFn subscriptionFn = SubscriptionFn{}, TState initialState = TState{});

  // Constructor: Initializes the store with an initial state.
  explicit StateStore(TState initialState);

  // Applies an update function to the state with optional arguments, and returns any additional output.
  template <typename TStateUpdateFn, typename... TArgs>
  auto apply(const UpdateName& updateName, const TStateUpdateFn& updateFn, TArgs&&... args);

  // Applies an update function to the state and passes its result to a transformation function, returning the
  // transformed output.
  template <typename TStateUpdateFn, typename TTransformFn, typename... TArgs>
  auto applyAndTransform(const UpdateName& updateName, const TStateUpdateFn& updateFn, const TTransformFn& transformFn,
                         TArgs&&... args);

  // Returns a callable that, when invoked, applies the update function to the state using the specified update name.
  template <typename TStateUpdateFn>
  [[nodiscard]] auto bind(UpdateName updateName, TStateUpdateFn&& updateFn);

  // Returns a callable that, when invoked, applies the update function and then the transform function to the result,
  // using the specified update name.
  template <typename TStateUpdateFn, typename TTransformFn>
  [[nodiscard]] auto bind(UpdateName updateName, TStateUpdateFn&& updateFn, TTransformFn&& transformFn);

  // Returns the current state value.
  [[nodiscard]] TState get_state() const;

 private:
  // Applies an update function to the state, updates the state, notifies the subscription (if present), and forwards
  // the update result.
  template <typename TStateUpdateFn, typename... TArgs>
  auto applyForwardingUpdateResult(const UpdateName& updateName, const TStateUpdateFn& updateFn, TArgs&&... args);

  SubscriptionFn subscriptionFn_;
  TState currentState_;
};

template <typename TState>
StateStore<TState>::StateStore(SubscriptionFn subscriptionFn, TState initialState)
    : subscriptionFn_{std::move(subscriptionFn)}, currentState_{std::move(initialState)} {}

template <typename TState>
StateStore<TState>::StateStore(TState initialState) : subscriptionFn_{}, currentState_{std::move(initialState)} {}

template <typename TState>
template <typename TStateUpdateFn, typename... TArgs>
auto StateStore<TState>::apply(const UpdateName& updateName, const TStateUpdateFn& updateFn, TArgs&&... args) {
  namespace fpd = ::funkypipes::details;

  auto updateResult = applyForwardingUpdateResult(updateName, updateFn, std::forward<decltype(args)>(args)...);

  if constexpr (fpd::IsTuple<decltype(updateResult)>) {
    auto [_, outputTuple] = fpd::separateTupleElements<0>(std::move(updateResult));

    return fpd::collapseOrForwardTuple(std::move(outputTuple));
  }
}

template <typename TState>
template <typename TStateUpdateFn, typename TTransformFn, typename... TArgs>
auto StateStore<TState>::applyAndTransform(const UpdateName& updateName, const TStateUpdateFn& updateFn,
                                           const TTransformFn& transformFn, TArgs&&... args) {
  namespace fpd = ::funkypipes::details;

  auto updateResult = applyForwardingUpdateResult(updateName, updateFn, std::forward<decltype(args)>(args)...);
  auto tupleAwareTransformFn = fpd::withTupleArgUnpacked(transformFn);
  return tupleAwareTransformFn(std::move(updateResult));
}

template <typename TState>
template <typename TStateUpdateFn>
[[nodiscard]] auto StateStore<TState>::bind(UpdateName updateName, TStateUpdateFn&& updateFn) {
  return [this, updateName_ = std::move(updateName), updateFn_ = std::forward<decltype(updateFn)>(updateFn)](
             auto&&... args) { return this->apply(updateName_, updateFn_, std::forward<decltype(args)>(args)...); };
}

template <typename TState>
template <typename TStateUpdateFn, typename TTransformFn>
[[nodiscard]] auto StateStore<TState>::bind(UpdateName updateName, TStateUpdateFn&& updateFn,
                                            TTransformFn&& transformFn) {
  return [this, updateName_ = std::move(updateName), updateFn_ = std::forward<decltype(updateFn)>(updateFn),
          transformFn_ = std::forward<decltype(transformFn)>(transformFn)](auto&&... args) {
    return this->applyAndTransform(updateName_, updateFn_, transformFn_, std::forward<decltype(args)>(args)...);
  };
}

template <typename TState>
[[nodiscard]] TState StateStore<TState>::get_state() const {
  return currentState_;
}

template <typename TState>
template <typename TStateUpdateFn, typename... TArgs>
auto StateStore<TState>::applyForwardingUpdateResult(const UpdateName& updateName, const TStateUpdateFn& updateFn,
                                                     TArgs&&... args) {
  namespace fpd = ::funkypipes::details;

  auto lastState = currentState_;

  auto updateResult = updateFn(currentState_, std::forward<TArgs>(args)...);

  if constexpr (fpd::IsTuple<decltype(updateResult)>) {
    currentState_ = std::get<0>(updateResult);
  } else {
    currentState_ = updateResult;
  }

  if (subscriptionFn_) {
    subscriptionFn_(updateName, std::move(lastState), currentState_);
  }

  return updateResult;
}

}  // namespace funkypipes

#endif  // FUNKYPIPES_STATE_STORE_HPP
