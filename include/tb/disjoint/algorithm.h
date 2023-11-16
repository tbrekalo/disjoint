#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace tb {

class subset {
  std::vector<std::uint8_t> state_;

 public:
  explicit subset(std::size_t size) : state_(size) {}

  subset() = default;
  ~subset() = default;

  subset(subset const&) = default;
  auto operator=(subset const&) -> subset& = default;

  subset(subset&&) = default;
  auto operator=(subset&&) -> subset& = default;

  friend auto operator<=>(subset const&, subset const&) = default;

  auto operator[](std::size_t idx) const noexcept -> bool {
    return state_[idx];
  }
  auto contains(std::size_t idx) const noexcept -> bool { return state_[idx]; }
  auto size() const noexcept -> std::size_t { return state_.size(); }

  auto insert(std::size_t idx) noexcept -> void { state_[idx] = 1; }
  auto erase(std::size_t idx) noexcept -> void { state_[idx] = 0; }
  auto clear() noexcept -> void { std::ranges::fill(state_, 0); }
};

auto binary_split(std::vector<subset>) -> std::vector<subset>;

}  // namespace tb
