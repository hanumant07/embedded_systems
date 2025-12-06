#include "aligned_malloc.h"
#include <cstdlib> // for malloc, free

void *aligned_malloc(size_t size, size_t alignment) {
  // TODO: Implement aligned_malloc
  // 1. Validate alignment (must be power of two)
  // 2. Allocate extra memory to ensure alignment and store offset
  // 3. Calculate aligned address
  // 4. Store offset for free

  if (alignment == 0 || (alignment & (alignment - 1)) == 0) {
    size_t aligned_size = size + alignment + sizeof(uintptr_t);
    void *ptr = malloc(aligned_size);
    if (!ptr) {
      return nullptr;
    }

    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t aligned_addr =
        (addr + alignment - 1 + sizeof(uintptr_t)) & ~(alignment - 1);
    uintptr_t orig_addr_store = aligned_addr - sizeof(uintptr_t);
    *reinterpret_cast<uintptr_t *>(orig_addr_store) = addr;
    return reinterpret_cast<void *>(aligned_addr);
  }

  return nullptr;
}

void aligned_free(void *ptr) {
  // TODO: Implement aligned_free
  // 1. Retrieve original pointer using stored offset
  // 2. Free original pointer
  uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
  uintptr_t orig_addr_store = addr - sizeof(uintptr_t);
  uintptr_t orig_addr = *reinterpret_cast<uintptr_t *>(orig_addr_store);
  free(reinterpret_cast<void *>(orig_addr));
}
