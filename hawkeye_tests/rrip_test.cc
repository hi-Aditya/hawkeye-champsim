#include "../replacement/hawkeye/rrip.h"

#include <cstddef>
#include <iostream>
#include <vector>

int main()
{
  std::vector<int> rrpv = {                                    //test vectors
    // for grader to fill
  };

  update_rrpv(rrpv, 0, Classification::CACHE_AVERSE, false);
  update_rrpv(rrpv, 1, Classification::CACHE_FRIENDLY, false);

  for (int v : rrpv)
    std::cout << v << " ";
  std::cout << "\n";
  std::cout << "victim: " << find_victim(rrpv) << "\n";

  std::vector<int> rrpv2 = {                                    //test vectors
    // for grader to fill
  };

  std::size_t v = find_victim(rrpv2);
  for (int x : rrpv2)
    std::cout << x << " ";
  std::cout << "\n";
  std::cout << "victim: " << v << "\n";
}
