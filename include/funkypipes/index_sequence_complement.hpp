#include <iostream>
#include <type_traits>
#include <utility>

// Helper to check if a value exists in an index_sequence
template <typename Seq, std::size_t Value>
struct contains;

template <std::size_t Value, std::size_t Head, std::size_t... Tail>
struct contains<std::index_sequence<Head, Tail...>, Value> {
  static constexpr bool value = (Value == Head) || contains<std::index_sequence<Tail...>, Value>::value;
};

template <std::size_t Value>
struct contains<std::index_sequence<>, Value> {
  static constexpr bool value = false;
};

// Helper to build the complementary index_sequence
template <std::size_t N, typename ExcludedSeq, std::size_t... Indices>
constexpr auto get_complement_impl(std::index_sequence<Indices...>) {
  // Create a tuple of optional indices, where indices in ExcludedSeq are set to void
  // and the rest are left as is.
  return std::index_sequence<(contains<ExcludedSeq, Indices>::value ? std::size_t{} : Indices)...>{};
}

// Filter out invalid indices (zeros) to construct the final sequence
template <std::size_t... Indices>
constexpr auto filter_valid_indices(std::index_sequence<Indices...>) {
  // Collect indices that are non-zero
  constexpr std::size_t valid_indices[] = {(Indices != 0 ? Indices : 0)...};
  constexpr std::size_t count = ((Indices != 0) + ...);  // Count of valid indices

  std::size_t result_indices[count]{};
  std::size_t idx = 0;
  for (std::size_t i = 0; i < sizeof...(Indices); ++i) {
    if (valid_indices[i] != 0) {
      result_indices[idx++] = valid_indices[i];
    }
  }

  // Create a new index_sequence with counted valid indices
  return [&]<std::size_t... I>(std::index_sequence<I...>) {
    return std::index_sequence<result_indices[I]...>{};
  }(std::make_index_sequence<count>{});
}

// Main function to compute complementary indices
template <std::size_t N, typename ExcludedSeq>
constexpr auto get_complement() {
  constexpr auto all_indices = std::make_index_sequence<N>{};
  constexpr auto complement_with_zeros = get_complement_impl<N, ExcludedSeq>(all_indices);
  return filter_valid_indices(complement_with_zeros);
}

// Example usage
int main() {
  using excluded_indices = std::index_sequence<1, 3, 5>;
  constexpr std::size_t N = 7;

  // Get the complement indices
  using complement_indices = decltype(get_complement<N, excluded_indices>());

  // Print the complement indices at runtime
  std::cout << "Complement indices: ";
  ((std::cout << complement_indices{} << ' '), ...);  // Cannot expand directly

  // To access the indices, we'll need to write a helper function
  auto print_indices = []<std::size_t... I>(std::index_sequence<I...>) { ((std::cout << I << ' '), ...); };
  print_indices(complement_indices{});

  std::cout << '\n';

  return 0;
}
