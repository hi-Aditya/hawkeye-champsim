#include "../replacement/hawkeye/predictor.h"
#include <iostream>
#include <vector>
#include <utility>
#include <cstdint>

int main() {
  HawkeyePredictor pred;
  std::vector<std::pair<uint64_t, bool>> train_events = {  };
  std::vector<uint64_t> query_pcs = {  };
  
  for (auto& event : train_events) {
    pred.train(event.first, event.second);
  }
  
  for (uint64_t pc : query_pcs) {
    std::cout << std::hex << pc << std::dec
              << ": counter=" << pred.get_counter(pc)
              << " predict=" << pred.predict(pc);
  }
}
