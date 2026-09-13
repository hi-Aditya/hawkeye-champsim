#ifndef HAWKEYE_RRIP_H
#define HAWKEYE_RRIP_H

#include <cstddef>
#include <vector>

// RRIP insertion/victim policy used by Hawkeye.
enum class Classification { CACHE_FRIENDLY, CACHE_AVERSE };

// Update the RRPVs for a hit or insertion.
void update_rrpv(std::vector<int>& rrpv,
                 std::size_t way,
                 Classification cls,
                 bool is_hit);

// Find the next victim, aging the set when needed.
std::size_t find_victim(std::vector<int>& rrpv);

#endif // HAWKEYE_RRIP_H
