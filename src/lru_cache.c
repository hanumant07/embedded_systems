#include "lru_cache.h"
#include <stdlib.h>
#include <string.h>

static int hash(lru_cache_t *cache, int key) {
  unsigned int h = (unsigned int)key;
  return h % cache->hash_size;
}

static lru_node_t *create_node(int key, int value) {
  lru_node_t *node = (lru_node_t *)malloc(sizeof(lru_node_t));
  if (node) {
    node->key = key;
    node->value = value;
    node->prev = node->next = node->hash_next = NULL;
  }
  return node;
}

static void remove_from_list(lru_cache_t *cache, lru_node_t *node) {
  if (node->prev)
    node->prev->next = node->next;
  else
    cache->head = node->next;

  if (node->next)
    node->next->prev = node->prev;
  else
    cache->tail = node->prev;
}

static void add_to_head(lru_cache_t *cache, lru_node_t *node) {
  node->next = cache->head;
  node->prev = NULL;
  if (cache->head)
    cache->head->prev = node;
  cache->head = node;
  if (!cache->tail)
    cache->tail = node;
}

lru_cache_t *lru_create(int capacity) {
  if (capacity <= 0)
    return NULL;
  lru_cache_t *cache = (lru_cache_t *)malloc(sizeof(lru_cache_t));
  if (!cache)
    return NULL;

  cache->capacity = capacity;
  cache->count = 0;
  cache->head = cache->tail = NULL;
  cache->hash_size = capacity * 2; // Simple heuristic
  cache->hash_table =
      (lru_node_t **)calloc(cache->hash_size, sizeof(lru_node_t *));

  if (!cache->hash_table) {
    free(cache);
    return NULL;
  }
  return cache;
}

void lru_free(lru_cache_t *cache) {
  if (!cache)
    return;
  lru_node_t *curr = cache->head;
  while (curr) {
    lru_node_t *next = curr->next;
    free(curr);
    curr = next;
  }
  free(cache->hash_table);
  free(cache);
}

int lru_get(lru_cache_t *cache, int key) {
  if (!cache)
    return -1;
  int h = hash(cache, key);
  lru_node_t *curr = cache->hash_table[h];

  while (curr) {
    if (curr->key == key) {
      remove_from_list(cache, curr);
      add_to_head(cache, curr);
      return curr->value;
    }
    curr = curr->hash_next;
  }
  return -1;
}

void lru_put(lru_cache_t *cache, int key, int value) {
  if (!cache)
    return;
  int h = hash(cache, key);
  lru_node_t *curr = cache->hash_table[h];

  // Check if key already exists
  while (curr) {
    if (curr->key == key) {
      curr->value = value;
      remove_from_list(cache, curr);
      add_to_head(cache, curr);
      return;
    }
    curr = curr->hash_next;
  }

  // Key doesn't exist, create new node
  if (cache->count >= cache->capacity) {
    // Evict LRU (tail)
    lru_node_t *evict = cache->tail;
    int evict_h = hash(cache, evict->key);
    lru_node_t **pp = &cache->hash_table[evict_h];
    while (*pp != evict)
      pp = &(*pp)->hash_next;
    *pp = evict->hash_next;

    remove_from_list(cache, evict);
    free(evict);
    cache->count--;
  }

  lru_node_t *node = create_node(key, value);
  if (!node)
    return;

  node->hash_next = cache->hash_table[h];
  cache->hash_table[h] = node;
  add_to_head(cache, node);
  cache->count++;
}
