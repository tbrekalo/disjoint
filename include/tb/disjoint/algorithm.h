#pragma once

#include <algorithm>
#include <ranges>
#include <span>

#include "tb/disjoint/subset.h"

namespace tb {

namespace detail {

struct subset_group_id {
  std::size_t subset_idx;
  std::size_t group_id;
};

}  // namespace detail

inline constexpr struct unique_t {
  // Returns a vector of unique subsets.
  //
  // The implementation is M * N * log(N) where M is the size of a subset and N
  // is the number of subses.
  static auto operator()(std::vector<subset> subsets) -> std::vector<subset> {
    std::ranges::sort(subsets);
    subsets.erase(std::ranges::unique(subsets).begin(), subsets.end());

    return subsets;
  }
} const unique;

inline constexpr struct unique_binary_t {
  // Returns a vector of unique subsets.
  //
  // The implementation is M * N * log(N) where M is the size of a subset and N
  // is the number of subsets.
  static auto operator()(std::vector<subset> subsets) -> std::vector<subset> {
    if (subsets.empty()) {
      return subsets;
    }

    auto curr_groups = std::vector<detail::subset_group_id>{};
    auto prev_groups = std::vector<detail::subset_group_id>{};

    curr_groups.resize(subsets.size());
    for (auto idx : std::views::iota(0uz, subsets.size())) {
      curr_groups[idx] = {.subset_idx = idx, .group_id = 0uz};
    }

    auto active_group_id = 0uz;
    auto active_group_size = 0uz;
    for (auto id : std::views::iota(0uz, subsets.front().size())) {
      std::swap(curr_groups, prev_groups);
      curr_groups.clear();

      active_group_id = 0uz;
      for (auto group_begin = prev_groups.begin();
           group_begin != prev_groups.end();) {
        auto group_end = std::find_if_not(
            group_begin, prev_groups.end(),
            [curr_group_id = group_begin->group_id](detail::subset_group_id sgi)
                -> bool { return sgi.group_id == curr_group_id; });

        for (auto contains : {true, false}) {
          active_group_size = 0uz;
          for (auto [subset_idx, _] : std::span{group_begin, group_end}) {
            if (subsets[subset_idx].contains(id) == contains) {
              curr_groups.push_back(detail::subset_group_id{
                  .subset_idx = subset_idx, .group_id = active_group_id});
              ++active_group_size;
            }
          }

          if (active_group_size > 0uz) {
            ++active_group_id;
          }
        }

        group_begin = group_end;
      }
    }

    curr_groups.erase(
        std::ranges::unique(curr_groups, {}, &detail::subset_group_id::group_id)
            .begin(),
        curr_groups.end());

    auto dst = std::vector<subset>(curr_groups.size());
    std::ranges::transform(
        curr_groups, dst.begin(),
        [&subsets](std::size_t subset_idx) -> subset {
          return std::move(subsets[subset_idx]);
        },
        &detail::subset_group_id::subset_idx);

    return dst;
  }
} const unique_binary;
}  // namespace tb
