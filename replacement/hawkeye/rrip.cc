#include "rrip.h"

#include <algorithm>

namespace{
constexpr int kMaxRRPV = 7;
constexpr int kFriendlyAgingCap = 6;
}

void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit){
  if(cls == Classification::CACHE_AVERSE) {
    rrpv[way] = kMaxRRPV;
    return;
  }
  rrpv[way] = 0;

  // On a friendly insertion, age the rest of the set 
  if (!is_hit) {
    for(std::size_t i = 0; i < rrpv.size(); ++i) {
      if (i != way && rrpv[i] < kFriendlyAgingCap) {
        ++rrpv[i];
      }
    }
  }
}

std::size_t find_victim(std::vector<int>& rrpv){
  // Prefer lines already marked for eviction 
  for(std::size_t i = 0; i < rrpv.size(); ++i) {
    if (rrpv[i] == kMaxRRPV) {
      return i;
    }
  }

  // Age the whole set until someone reaches the eviction threshold 
  const int max_val = *std::max_element(rrpv.begin(), rrpv.end());
  const int delta = kMaxRRPV - max_val;

  for(auto& v : rrpv){
    v += delta;
  }

  // Lowest way wins ties 
  for (std::size_t i = 0; i < rrpv.size(); ++i){
    if (rrpv[i] == kMaxRRPV) {
      return i;
    }
  }
  return 0;
}
