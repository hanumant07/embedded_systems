#include "find_first_set_bit.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_find_first_set_bit() {
  std::cout << "Testing find_first_set_bit..." << std::endl;

  // 1. Zero check
  assert(find_first_set_bit(0) == -1);
  std::cout << "  [PASS] Zero check" << std::endl;

  // 2. Single bit set checks checks across the range
  for (int i = 0; i < 64; ++i) {
    uint64_t bitmap = (uint64_t)1 << i;
    int result = find_first_set_bit(bitmap);
    if (result != i) {
      std::cout << "  [FAIL] Expected " << i << ", got " << result << std::endl;
      assert(result == i);
    }
  }
  std::cout << "  [PASS] Single bit checks (0-63)" << std::endl;

  // 3. Multiple bits set (lowest should be returned)
  {
    // Bits 2, 10, 60 set. Lowest is 2.
    uint64_t bitmap =
        ((uint64_t)1 << 2) | ((uint64_t)1 << 10) | ((uint64_t)1 << 60);
    assert(find_first_set_bit(bitmap) == 2);
    std::cout << "  [PASS] Multiple bits (2, 10, 60) -> 2" << std::endl;
  }

  // 4. Multiple bits, lowest is high up
  {
    // Bits 40, 50. Lowest is 40.
    uint64_t bitmap = ((uint64_t)1 << 40) | ((uint64_t)1 << 50);
    assert(find_first_set_bit(bitmap) == 40);
    std::cout << "  [PASS] Multiple bits (40, 50) -> 40" << std::endl;
  }

  std::cout << "find_first_set_bit tests passed!" << std::endl;
}
