#ifndef HAWKEYE_OPTGEN_H
#define HAWKEYE_OPTGEN_H

#include <cstddef>
#include <cstdint>
#include <deque>
#include <unordered_map>
#include <vector>

// Section 3.1 of the Hawkeye paper.
//
// address is a cache block address, not a byte address.
// Each set keeps a sliding history of history_multiplier * associativity
// accesses. occupancy stores the current occupancy for each history entry.
//
// An access is a hit if interval since its previous reference never
// reaches associativity. Misses don't update occupancy.
// References with no previous entry in the current window are misses.

class OPTgen {
 public:
  OPTgen(std::size_t num_sets,
         std::size_t associativity,
         std::size_t history_multiplier = 8);

  bool access(std::size_t set_idx, uint64_t address);

 private:
  struct SetState {
    // occupancy.front() is base_time.
    std::deque<int> occupancy;
    std::size_t base_time = 0;
    std::size_t next_time = 0;

    // Addresses whose last reference is still in the window.
    std::unordered_map<uint64_t, std::size_t> last_seen;
  };

  std::size_t associativity_;
  std::size_t window_len_;
  std::vector<SetState> sets_;
};

#endif
