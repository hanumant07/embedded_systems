#include "modify_reg.h"
#include <cassert>
#include <iomanip> // for std::hex
#include <iostream>

void test_modify_reg() {
  std::cout << "Testing modify_reg..." << std::endl;

  // 1. **SET Operation**
  {
    uint32_t val = 0x00000000;
    // Set bit 0
    modify_reg(&val, 0x01, SET);
    assert(val == 0x00000001);

    // Set bit 4 and 5 (0x30)
    modify_reg(&val, 0x30, SET);
    assert(val == 0x00000031); // 1 | 0x30 = 0x31

    // Set already set bit (idempotency)
    modify_reg(&val, 0x01, SET);
    assert(val == 0x00000031);
    std::cout << "  [PASS] SET operation" << std::endl;
  }

  // 2. **CLEAR Operation**
  {
    uint32_t val = 0xFFFFFFFF;
    // Clear bit 0
    modify_reg(&val, 0x01, CLEAR);
    assert(val == 0xFFFFFFFE);

    // Clear top 16 bits
    modify_reg(&val, 0xFFFF0000, CLEAR);
    assert(val == 0x0000FFFE);

    // Clear already clear bit
    modify_reg(&val, 0x01, CLEAR);
    assert(val == 0x0000FFFE);
    std::cout << "  [PASS] CLEAR operation" << std::endl;
  }

  // 3. **TOGGLE Operation**
  {
    uint32_t val = 0x00000000;
    // Toggle bit 0 (0 -> 1)
    modify_reg(&val, 0x01, TOGGLE);
    assert(val == 0x00000001);

    // Toggle bit 0 (1 -> 0)
    modify_reg(&val, 0x01, TOGGLE);
    assert(val == 0x00000000);

    // Toggle mixed
    // 0xF0F0 (val) XOR 0xFFFF (mask) = 0x0F0F
    val = 0xF0F0;
    modify_reg(&val, 0xFFFF, TOGGLE);
    assert(val == 0x0F0F);
    std::cout << "  [PASS] TOGGLE operation" << std::endl;
  }

  // 4. **No-op Mask**
  {
    uint32_t val = 0x12345678;
    modify_reg(&val, 0, SET);
    assert(val == 0x12345678);
    modify_reg(&val, 0, CLEAR);
    assert(val == 0x12345678);
    modify_reg(&val, 0, TOGGLE);
    assert(val == 0x12345678);
    std::cout << "  [PASS] Zero mask (no-op)" << std::endl;
  }

  std::cout << "modify_reg tests passed!" << std::endl;
}
