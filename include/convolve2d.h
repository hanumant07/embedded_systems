#ifndef CONVOLVE2D_H
#define CONVOLVE2D_H

#include <cstdint>

/**
 * @brief Performs a 2D convolution on a grayscale image.
 *
 * @param input Pointer to the input image data (row-major).
 * @param output Pointer to the output image data (row-major). Must be allocated
 * by the caller.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param kernel Pointer to the square kernel data (row-major).
 * @param kernelSize The dimension of the square kernel (e.g., 3 for a 3x3
 * kernel).
 *
 * The output image will have the same dimensions as the input.
 * Boundary conditions are handled by zero-padding.
 * The result is clamped to [0, 255].
 */
void convolve2D(const uint8_t *input, uint8_t *output, int width, int height,
                const int *kernel, int kernelSize);

/**
 * @brief Performs a 2D convolution using a separable kernel (two 1D passes).
 *
 * @param input Pointer to the input image data.
 * @param output Pointer to the output image data.
 * @param width Width of the image.
 * @param height Height of the image.
 * @param kernelX Horizontal 1D kernel.
 * @param kernelSizeX Size of the horizontal kernel.
 * @param kernelY Vertical 1D kernel.
 * @param kernelSizeY Size of the vertical kernel.
 *
 * Optimization: Reduces complexity from O(K*K) to O(K+K) per pixel.
 */
void convolve2DSeparable(const uint8_t *input, uint8_t *output, int width,
                         int height, const int *kernelX, int kernelSizeX,
                         const int *kernelY, int kernelSizeY);

#endif // CONVOLVE2D_H
