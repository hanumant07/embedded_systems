#ifndef MY_MEMMOVE_H
#define MY_MEMMOVE_H

#include <stddef.h> // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Moves n bytes from src to dest.
 * Handles overlapping memory regions.
 * Optimized for word-aligned copies.
 *
 * @param dest Destination buffer
 * @param src Source buffer
 * @param n Number of bytes to copy
 * @return void* Pointer to dest
 */
void *my_memmove(void *dest, const void *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif // MY_MEMMOVE_H
