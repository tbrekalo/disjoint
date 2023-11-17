#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace tb {

class subset {
  std::vector<std::uint8_t> state_;

 public:
  constexpr explicit subset(std::size_t size) : state_(size) {}

  constexpr subset() = default;
  constexpr ~subset() = default;

  constexpr subset(subset const&) = default;
  constexpr auto operator=(subset const&) -> subset& = default;

  constexpr subset(subset&&) = default;
  constexpr auto operator=(subset&&) -> subset& = default;

  constexpr friend auto operator<=>(subset const&, subset const&) = default;

  constexpr auto operator[](std::size_t idx) const noexcept -> bool {
    return state_[idx];
  }
  constexpr auto contains(std::size_t idx) const noexcept -> bool {
    return state_[idx];
  }
  constexpr auto size() const noexcept -> std::size_t { return state_.size(); }

  constexpr auto insert(std::size_t idx) noexcept -> void { state_[idx] = 1; }
  constexpr auto erase(std::size_t idx) noexcept -> void { state_[idx] = 0; }
  constexpr auto clear() noexcept -> void { std::ranges::fill(state_, 0); }
};

auto binary_split(std::vector<subset>) -> std::vector<subset>;

}  // namespace tb
