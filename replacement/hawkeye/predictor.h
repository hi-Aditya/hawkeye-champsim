#ifndef HAWKEYE_PREDICTOR_H
#define HAWKEYE_PREDICTOR_H

#include <cstddef>
#include <cstdint>
#include <vector>

class HawkeyePredictor {
public:
  HawkeyePredictor(std::size_t num_entries = 8192, int counter_bits = 3);
  
  void train(uint64_t pc, bool opt_hit);
  
  bool predict(uint64_t pc) const;
  
  int get_counter(uint64_t pc) const;

private:
  std::size_t index_of(uint64_t pc) const;
  std::size_t num_entries_;
  int max_value_;
  int init_value_;
  std::vector<int> counters_;
};

#endif // HAWKEYE_PREDICTOR_H
