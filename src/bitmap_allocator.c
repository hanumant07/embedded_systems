#include "bitmap_allocator.h"

int bitmap_alloc_first_free(uint32_t *bitmap, size_t size_in_bits) {
  if (!bitmap)
    return -1;

  size_t num_words = (size_in_bits + 31) / 32;

  for (size_t i = 0; i < num_words; i++) {
    uint32_t word = bitmap[i];

    // If word is not all 1s (0xFFFFFFFF), there's at least one 0 bit
    if (word != 0xFFFFFFFF) {
      // Find the first 0 bit in the current word
      for (int bit = 0; bit < 32; bit++) {
        size_t index = i * 32 + bit;
        if (index >= size_in_bits)
          break;

        if (!(word & (1U << bit))) {
          // Set the bit
          bitmap[i] |= (1U << bit);
          return (int)index;
        }
      }
    }
  }

  return -1;
}

void bitmap_free(uint32_t *bitmap, int index) {
  if (!bitmap || index < 0)
    return;

  size_t word_idx = (size_t)index / 32;
  int bit_idx = index % 32;

  bitmap[word_idx] &= ~(1U << bit_idx);
}
