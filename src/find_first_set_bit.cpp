#include "find_first_set_bit.h"

int find_first_set_bit(uint64_t bitmap) {
  if (bitmap == 0) {
    return -1;
  }

  int n = 0;

  // Binary search (divide and conquer) for the lowest set bit.
  // If the lower half is empty, the bit must be in the upper half.
  // Move to the upper half (shift) and update the index count.

  if ((bitmap & 0xFFFFFFFF) == 0) {
    n += 32;
    bitmap >>= 32;
  }
  if ((bitmap & 0xFFFF) == 0) {
    n += 16;
    bitmap >>= 16;
  }
  if ((bitmap & 0xFF) == 0) {
    n += 8;
    bitmap >>= 8;
  }
  if ((bitmap & 0xF) == 0) {
    n += 4;
    bitmap >>= 4;
  }
  if ((bitmap & 0x3) == 0) {
    n += 2;
    bitmap >>= 2;
  }
  if ((bitmap & 0x1) == 0) {
    n += 1;
  }

  return n;
}
