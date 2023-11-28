#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <array>
#include <ranges>

#include "doctest/doctest.h"
#include "tb/disjoint/algorithm.h"

static auto create_min_sample() {
  static constexpr auto SUBSET_SIZE = 10uz;
  auto initilize_subset = [](auto... indices) constexpr -> tb::subset {
    auto dst = tb::subset(SUBSET_SIZE);
    (dst.insert(indices), ...);

    return dst;
  };

  auto ret = std::vector<tb::subset>{
      initilize_subset(1, 3, 7),
      initilize_subset(1, 3, 5),
      initilize_subset(0, 2, 4),
  };

  std::ranges::sort(ret);
  return ret;
}

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

TEST_CASE_TEMPLATE("unique", T, decltype(tb::unique),
                   decltype(tb::unique_binary), decltype(tb::unique_ankerl)) {
  static constexpr auto unique_algo = T{};
  static auto UNIQUE_SUBSETS = create_min_sample();

  auto const create_subsets = [](std::size_t n_repeats) {
    auto dst = std::vector<tb::subset>{};
    for (auto _ : std::views::iota(0uz, n_repeats)) {
      dst.insert(dst.end(), UNIQUE_SUBSETS.begin(), UNIQUE_SUBSETS.end());
    }

    return dst;
  };

  static auto constexpr N_REPEATS = 4uz;

  auto const subsets = create_subsets(N_REPEATS);
  auto const unique_subsets = [&] {
    auto dst = unique_algo(subsets);
    std::ranges::sort(dst);

    return dst;
  }();

  REQUIRE_EQ(subsets.size(), N_REPEATS * UNIQUE_SUBSETS.size());
  REQUIRE_EQ(unique_subsets, UNIQUE_SUBSETS);
}
