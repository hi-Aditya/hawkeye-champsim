#include "predictor.h"

namespace {
  bool is_power_of_two(std::size_t n) {
    return n != 0 && (n & (n - 1)) == 0;
  }
}

HawkeyePredictor::HawkeyePredictor(std::size_t num_entries, int counter_bits)
    : num_entries_(num_entries > 0 ? num_entries : 1),
      max_value_((1 << (counter_bits > 0 ? counter_bits : 1)) - 1),
      init_value_(1 << ((counter_bits > 0 ? counter_bits : 1) - 1)),
      counters_(num_entries_, init_value_) {}

std::size_t HawkeyePredictor::index_of(uint64_t pc) const {
  const uint64_t hashed_pc = pc ^ (pc >> 12);
  if (is_power_of_two(num_entries_)) {
    return static_cast<std::size_t>(hashed_pc) & (num_entries_ - 1);
  }
  return static_cast<std::size_t>(hashed_pc % num_entries_);
}

void HawkeyePredictor::train(uint64_t pc, bool opt_hit) {
  int& counter = counters_[index_of(pc)];
  if (opt_hit) {
    if (counter < max_value_) {
      ++counter;
    }
  } else {
    if (counter > 0) {
      --counter;
    }
  }
}

bool HawkeyePredictor::predict(uint64_t pc) const {
  return counters_[index_of(pc)] >= init_value_;
}

int HawkeyePredictor::get_counter(uint64_t pc) const {
  return counters_[index_of(pc)];
}
