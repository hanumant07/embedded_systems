#include "lru_cache.h"
#include <assert.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_lru_basic() {
  lru_cache_t *cache = lru_create(2);
  assert(cache != NULL);

  lru_put(cache, 1, 10);
  lru_put(cache, 2, 20);

  // Get 1, should be 10 and move to head
  assert(lru_get(cache, 1) == 10);

  // Put 3, should evict 2 (LRU)
  lru_put(cache, 3, 30);

  assert(lru_get(cache, 2) == -1); // 2 evicted
  assert(lru_get(cache, 1) == 10); // 1 still there
  assert(lru_get(cache, 3) == 30); // 3 still there

  lru_free(cache);
  printf("test_lru_basic passed!\n");
}

void test_lru_update() {
  lru_cache_t *cache = lru_create(2);

  lru_put(cache, 1, 10);
  lru_put(cache, 1, 15); // Update

  assert(lru_get(cache, 1) == 15);

  lru_free(cache);
  printf("test_lru_update passed!\n");
}

void test_lru() {
  test_lru_basic();
  test_lru_update();
  printf("All LRU tests passed!\n");
}

#ifdef __cplusplus
}
#endif
