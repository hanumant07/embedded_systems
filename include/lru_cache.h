#ifndef LRU_CACHE_H
#define LRU_CACHE_H

/* * 3. LEAST RECENTLY USED (LRU) CACHE
 * Context: Implement a cache for disk pages or network routes.
 * Constraints: O(1) put and O(1) get. Requires combining a Hash Map and a
 * Doubly Linked List.
 */
typedef struct lru_node {
  int key;
  int value;
  struct lru_node *prev;
  struct lru_node *next;
  struct lru_node *hash_next; // Collision handling for simple chaining
} lru_node_t;

typedef struct {
  int capacity;
  int count;
  lru_node_t *head;
  lru_node_t *tail;
  lru_node_t **hash_table;
  int hash_size;
} lru_cache_t;

#ifdef __cplusplus
extern "C" {
#endif

void lru_put(lru_cache_t *cache, int key, int value);
int lru_get(lru_cache_t *cache, int key);

/**
 * lru_create - Initializes a new LRU cache with a fixed capacity.
 * @capacity: Max number of items the cache can hold.
 */
lru_cache_t *lru_create(int capacity);

/**
 * lru_free - Releases all memory associated with the cache.
 */
void lru_free(lru_cache_t *cache);

#ifdef __cplusplus
}
#endif

#endif // LRU_CACHE_H
