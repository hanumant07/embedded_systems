#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include <cstdint>

/**
 * @brief Finds connected components in a binary image.
 *
 * @param input Pointer to the input image data (row-major). Non-zero pixels are
 * treated as objects.
 * @param labels Pointer to the output labels array (row-major). Must be
 * allocated by caller. Same dimensions as input.
 * @param width Width of the image.
 * @param height Height of the image.
 * @return The number of connected components found.
 */
int findConnectedComponents(const uint8_t *input, int *labels, int width,
                            int height);

#endif // CONNECTED_COMPONENTS_H
