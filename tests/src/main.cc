#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <ranges>

#include "doctest/doctest.h"
#include "tb/disjoint/algorithm.h"

TEST_CASE("subset") {
  SUBCASE("zero-sized") { REQUIRE_EQ(tb::subset{}.size(), 0uz); }

  static constexpr auto SUBSET_SIZE = 10uz;
  auto subset = tb::subset(SUBSET_SIZE);

  SUBCASE("empty") {
    for (auto idx : std::views::iota(0uz, SUBSET_SIZE)) {
      CHECK_EQ(subset[idx], false);
      CHECK_EQ(subset.contains(idx), false);
    }
  }
}

TEST_CASE("binary-split-empty") {
  REQUIRE_EQ(tb::binary_split(std::vector<tb::subset>{}),
             std::vector<tb::subset>{});
}
