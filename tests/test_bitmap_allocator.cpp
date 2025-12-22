#include "bitmap_allocator.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_bitmap_allocator() {
  std::cout << "Testing bitmap_allocator..." << std::endl;

  const size_t BITMAP_SIZE = 100;
  uint32_t bitmap[4] = {0}; // 4 * 32 = 128 bits, enough for 100

  // 1. Basic allocation
  int idx0 = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx0 == 0);
  assert(bitmap[0] == 0x1);
  std::cout << "  [PASS] Allocated bit 0" << std::endl;

  int idx1 = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx1 == 1);
  assert(bitmap[0] == 0x3);
  std::cout << "  [PASS] Allocated bit 1" << std::endl;

  // 2. Freeing and re-allocating
  bitmap_free(bitmap, 0);
  assert(bitmap[0] == 0x2);
  std::cout << "  [PASS] Freed bit 0" << std::endl;

  int idx0_new = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx0_new == 0);
  assert(bitmap[0] == 0x3);
  std::cout << "  [PASS] Re-allocated bit 0" << std::endl;

  // 3. Spanning multiple words
  // Fill first word
  bitmap[0] = 0xFFFFFFFF;
  int idx32 = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx32 == 32);
  assert(bitmap[1] == 0x1);
  std::cout << "  [PASS] Allocated across word boundary (bit 32)" << std::endl;

  // 4. Full bitmap check
  // Set all bits up to 100
  for (int i = 0; i < 3; i++)
    bitmap[i] = 0xFFFFFFFF;
  bitmap[3] = 0xFFFFFFFF; // Set all 128 bits

  int idx_full = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx_full == -1);
  std::cout << "  [PASS] Full bitmap returns -1" << std::endl;

  // 5. Boundary check
  // Clear bit 99
  bitmap_free(bitmap, 99);
  int idx99 = bitmap_alloc_first_free(bitmap, BITMAP_SIZE);
  assert(idx99 == 99);
  std::cout << "  [PASS] Allocated boundary bit 99" << std::endl;

  std::cout << "bitmap_allocator tests passed!" << std::endl;
}
