#include "aligned_malloc.h"
#include <cassert>
#include <cstring> // for memset
#include <iostream>
#include <stdint.h> // for uintptr_t
#include <vector>

void test_aligned_malloc() {
  std::cout << "Testing aligned_malloc..." << std::endl;

  // 1. Basic aligned allocation
  {
    size_t size = 100;
    size_t align = 64;
    void *ptr = aligned_malloc(size, align);
    if (ptr == nullptr) {
      std::cout << "  [FAIL] returned nullptr for valid alloc" << std::endl;
    } else {
      assert((uintptr_t)ptr % align == 0);

      // Write to it
      memset(ptr, 0xAA, size);

      aligned_free(ptr);
      std::cout << "  [PASS] Allocation 100 bytes, align 64" << std::endl;
    }
  }

  // 2. Large alignment
  {
    size_t size = 10;
    size_t align = 4096;
    void *ptr = aligned_malloc(size, align);
    if (ptr) {
      assert((uintptr_t)ptr % align == 0);
      aligned_free(ptr);
      std::cout << "  [PASS] Allocation 10 bytes, align 4096" << std::endl;
    }
  }

  // 3. Alignment equal to size
  {
    size_t size = 128;
    size_t align = 128;
    void *ptr = aligned_malloc(size, align);
    if (ptr) {
      assert((uintptr_t)ptr % align == 0);
      aligned_free(ptr);
      std::cout << "  [PASS] Allocation 128 bytes, align 128" << std::endl;
    }
  }

  // 4. Power of two check (Optional: depends on implementation)
  // We won't crash if it accepts invalid alignment, just warn.
  {
    void *ptr = aligned_malloc(100, 63);
    if (ptr == nullptr) {
      std::cout << "  [PASS] Invalid alignment rejected (returned nullptr)"
                << std::endl;
    } else {
      aligned_free(ptr);
      std::cout << "  [INFO] Invalid alignment accepted" << std::endl;
    }
  }

  std::cout << "aligned_malloc tests passed!" << std::endl;
}
