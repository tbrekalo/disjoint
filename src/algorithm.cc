#include "tb/disjoint/algorithm.h"

namespace tb {

auto unique(std::vector<subset> subsets) -> std::vector<subset> {
  std::ranges::sort(subsets);
  subsets.erase(std::ranges::unique(subsets).begin(), subsets.end());

  return subsets;
}

}  // namespace tb
