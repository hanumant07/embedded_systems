#ifndef BITMAP_ALLOCATOR_H
#define BITMAP_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Manage allocation of resources (like PIDs or pages) using a bitmap.
 *        Finds the first free bit (0), sets it to 1, and returns the index.
 *
 * @param bitmap Pointer to the array of uint32_t representing the bitmap.
 * @param size_in_bits Total number of bits in the bitmap.
 * @return int Index of the allocated bit, or -1 if no bits are free.
 */
int bitmap_alloc_first_free(uint32_t *bitmap, size_t size_in_bits);

/**
 * @brief Frees an allocated bit (sets it back to 0).
 *
 * @param bitmap Pointer to the array of uint32_t representing the bitmap.
 * @param index The index of the bit to free.
 */
void bitmap_free(uint32_t *bitmap, int index);

#ifdef __cplusplus
}
#endif

#endif // BITMAP_ALLOCATOR_H
