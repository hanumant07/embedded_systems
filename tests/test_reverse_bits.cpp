#include "reverse_bits.h"
#include <bitset>
#include <cassert>
#include <iostream>

void test_reverse_bits() {
  std::cout << "Testing reverse_bits..." << std::endl;

  // Test cases
  assert(reverse_bits(0x00000000) == 0x00000000); // 0 -> 0
  assert(reverse_bits(0xFFFFFFFF) == 0xFFFFFFFF); // All 1s -> All 1s
  assert(reverse_bits(0x0000FFFF) == 0xFFFF0000); // Low half -> High half
  assert(reverse_bits(0x80000000) == 0x00000001); // MSB -> LSB
  assert(reverse_bits(0x12345678) == 0x1E6A2C48); // Specific pattern

  std::cout << "reverse_bits tests passed!" << std::endl;
}
