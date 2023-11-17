#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
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

  SUBCASE("contains-single") {
    static constexpr auto ITEM_IDX = 5;
    subset.insert(ITEM_IDX);

    auto not_contained = std::views::iota(0uz, SUBSET_SIZE) |
                         std::views::filter([](std::size_t idx) -> bool {
                           return idx != ITEM_IDX;
                         });

    CHECK(subset.contains(ITEM_IDX));
    for (auto idx : not_contained) {
      CHECK_FALSE(subset.contains(idx));
    }
  }

  SUBCASE("contains-multi") {
    static constexpr auto CONTAINED_INDICES =
        std::array<std::size_t, 3>{1uz, 3uz, 7uz};
    for (auto idx : CONTAINED_INDICES) {
      subset.insert(idx);
    }

    for (auto idx : CONTAINED_INDICES) {
      CHECK(subset.contains(idx));
    }

    SUBCASE("clear") {
      subset.clear();
      for (auto idx : std::views::iota(0uz, SUBSET_SIZE)) {
        CHECK_FALSE(subset.contains(idx));
      }
    }

    auto not_contained = std::views::iota(0uz, SUBSET_SIZE) |
                         std::views::filter([](std::size_t idx) -> bool {
                           return std::ranges::find(CONTAINED_INDICES, idx) ==
                                  CONTAINED_INDICES.end();
                         });

    for (auto idx : not_contained) {
      CHECK_FALSE(subset.contains(idx));
    }

    SUBCASE("erase") {
      subset.erase(CONTAINED_INDICES.back());
      auto contained_view =
          std::views::take(CONTAINED_INDICES, CONTAINED_INDICES.size() - 1uz);

      CHECK_FALSE(subset.contains(CONTAINED_INDICES.back()));
      for (auto idx : contained_view) {
        CHECK(subset.contains(idx));
      }
    }
  }
}

TEST_CASE("binary-split-empty") {
  REQUIRE_EQ(tb::binary_split(std::vector<tb::subset>{}),
             std::vector<tb::subset>{});
}
