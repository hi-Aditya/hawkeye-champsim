#include "hawkeye.h"

hawkeye::hawkeye(CACHE* cache) : hawkeye(cache, cache->NUM_SET, cache->NUM_WAY) {}

hawkeye::hawkeye(CACHE* cache, long sets, long ways) : replacement(cache), NUM_WAY(ways), NUM_SET(sets), optgen(static_cast<std::size_t>(sets), static_cast<std::size_t>(ways)), predictor(), rrpv(static_cast<std::size_t>(sets), std::vector<int>(static_cast<std::size_t>(ways), 7)){}

long hawkeye::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip, champsim::address full_addr, access_type type){  
  auto& set_rrpv = rrpv.at(static_cast<std::size_t>(set));  
  // Using the RRIP helper function for finding the victim.  
  return static_cast<long>(::find_victim(set_rrpv));
}

void hawkeye::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr, access_type type, uint8_t hit){  
  const auto block_addr = champsim::block_number{full_addr}.to<uint64_t>();  
  const auto pc = ip.to<uint64_t>();  
  auto prev_it = last_pc_for_block.find(block_addr);  
  const bool has_prev = (prev_it != last_pc_for_block.end());  
  const uint64_t prev_pc = has_prev ? prev_it->second : 0;  
  // OPTgen processes every access, including misses.  
  const bool opt_hit = optgen.access(static_cast<std::size_t>(set), block_addr);  

  // Training with the PC from the previous reference.  
  if (has_prev){    
    predictor.train(prev_pc, opt_hit);  
  }  
  last_pc_for_block[block_addr] = pc;  
  
  // Avoid modifying the way on a miss as it may be NUM_WAY.  
  if (hit){    
    const auto cls = predictor.predict(pc) ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE;    
    auto& set_rrpv = rrpv.at(static_cast<std::size_t>(set));    
    update_rrpv(set_rrpv, static_cast<std::size_t>(way), cls, true);  
  }
}

void hawkeye::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr, access_type type){  
  const auto pc = ip.to<uint64_t>();  
  const auto cls = predictor.predict(pc) ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE;  
  auto& set_rrpv = rrpv.at(static_cast<std::size_t>(set));
  update_rrpv(set_rrpv, static_cast<std::size_t>(way), cls, /*is_hit=*/false);
}

