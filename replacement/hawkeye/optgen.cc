#include "optgen.h"

OPTgen::OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier)
    : associativity_(associativity), window_len_(history_multiplier * associativity), sets_(num_sets)
{
}

bool OPTgen::access(std::size_t set_idx, uint64_t address)
{
  SetState& s = sets_.at(set_idx);

  // Trivial miss, zero tracked history
  if (window_len_ == 0) {
    s.last_seen[address] = s.next_time++;
    return false;
  }

  // Add curr access to history
  const std::size_t t = s.next_time++;
  s.occupancy.push_back(0);

  // Keep only last window_len_ accesses
  while (s.occupancy.size() > window_len_) {
    s.occupancy.pop_front();
    ++s.base_time;
  }

  // Find any previous access to this addr if in history
  auto it = s.last_seen.find(address);
  const bool has_prev = (it != s.last_seen.end()) && (it->second >= s.base_time);

  bool hit = false;
  if (has_prev) {
    const std::size_t prev_t = it->second;
    const std::size_t begin_idx = prev_t - s.base_time;
    const std::size_t end_idx = t - s.base_time;

    bool all_below_capacity = true;
    for (std::size_t i = begin_idx; i < end_idx; ++i) {
      if (s.occupancy[i] >= static_cast<int>(associativity_)) {
        all_below_capacity = false;
        break;
      }
    }

    if (all_below_capacity) {
      for (std::size_t i = begin_idx; i < end_idx; ++i) {
        ++s.occupancy[i];
      }
      hit = true;
    }
  }

  // Most recent reference to addr
  s.last_seen[address] = t;

  return hit;
}
