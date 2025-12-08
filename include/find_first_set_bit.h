#ifndef FIND_FIRST_SET_BIT_H
#define FIND_FIRST_SET_BIT_H

#include <stdint.h> // for uint64_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Finds the index of the lowest set bit (0-63).
 *        Represents finding the highest priority interrupt line.
 *        Implementation should NOT use built-in instructions like
 * __builtin_ctz.
 *
 * @param bitmap The 64-bit interrupt bitmap.
 * @return int Index of the lowest set bit (0-63), or -1 if no bits are set.
 */
int find_first_set_bit(uint64_t bitmap);

#ifdef __cplusplus
}
#endif

#endif // FIND_FIRST_SET_BIT_H
