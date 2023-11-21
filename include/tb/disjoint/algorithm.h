#pragma once

#include "tb/disjoint/subset.h"

namespace tb {

// Returns a vector of unique subsets.
auto unique(std::vector<subset> subsets) -> std::vector<subset>;

}  // namespace tb
