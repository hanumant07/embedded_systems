#include "reverse_bits.h"
#include <array>

// Compile-time helper to reverse a byte
// We use a helper struct or just a free function marked constexpr
constexpr uint8_t reverse_byte(uint8_t n) {
  n = (n & 0xF0) >> 4 | (n & 0x0F) << 4;
  n = (n & 0xCC) >> 2 | (n & 0x33) << 2;
  n = (n & 0xAA) >> 1 | (n & 0x55) << 1;
  return n;
}

// Generate table at compile time
constexpr std::array<uint8_t, 256> generate_table() {
  std::array<uint8_t, 256> table{};
  for (int i = 0; i < 256; ++i) {
    table[i] = reverse_byte(static_cast<uint8_t>(i));
  }
  return table;
}

// The table is generated at compile time and stored in the data segment
static constexpr auto LUT = generate_table();

uint32_t reverse_bits(uint32_t n) {
  return (LUT[n & 0xFF] << 24) | (LUT[(n >> 8) & 0xFF] << 16) |
         (LUT[(n >> 16) & 0xFF] << 8) | (LUT[(n >> 24) & 0xFF]);
}