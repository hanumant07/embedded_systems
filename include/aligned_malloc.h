#ifndef ALIGNED_MALLOC_H
#define ALIGNED_MALLOC_H

#include <stddef.h> // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocates size bytes of uninitialized storage whose alignment is
 * specified by alignment.
 *
 * @param size Number of bytes to allocate.
 * @param alignment The alignment value, which must be a power of two.
 * @return void* On success, returns a pointer to the allocated memory. On
 * error, returns NULL.
 */
void *aligned_malloc(size_t size, size_t alignment);

/**
 * @brief Frees a block of memory that was allocated with aligned_malloc.
 *
 * @param ptr Pointer to the memory block to be freed.
 */
void aligned_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // ALIGNED_MALLOC_H
