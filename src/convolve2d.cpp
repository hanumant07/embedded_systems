#include "convolve2d.h"
#include <vector>

// convolve2D implements a discrete spatial 2D convolution.
// Algorithm overview:
// 1. Sliding Window: We iterate over every pixel of the input image.
// 2. Kernel Application: For each pixel, we overlay the kernel centered at that
// pixel.
// 3. Accumulation: We sum the products of overlapping image pixels and kernel
// weights.
// 4. Boundary Handling: We use Zero-Padding. Pixels "outside" the image
// boundaries are treated as having value 0.
// 5. Clamping: The final sum is clamped to the valid uint8_t range [0, 255] to
// prevent overflow/underflow artifacts.

void convolve2D(const uint8_t *input, uint8_t *output, int width, int height,
                const int *kernel, int kernelSize) {
  // Calculate the radius of the kernel (kHalf).
  // For a 3x3 kernel, kernelSize is 3, kHalf is 1.
  // The kernel indices will range from -kHalf to +kHalf relative to the center.
  int kHalf = kernelSize / 2;

  // Iterate over every pixel in the input image (y, x)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int sum = 0; // Accumulator for the weighted sum

      // Iterate over the kernel window (ky, kx)
      for (int ky = 0; ky < kernelSize; ++ky) {
        for (int kx = 0; kx < kernelSize; ++kx) {
          // Calculate the absolute position of the neighbor pixel
          // x: current pixel x-coordinate
          // kx: kernel x-coordinate (0 to kernelSize-1)
          // kHalf: offset to center the kernel
          int pixelX = x + (kx - kHalf);
          int pixelY = y + (ky - kHalf);

          // Boundary Check (Zero-Padding):
          // If the neighbor pixel is within the image bounds, include it in the
          // sum. Otherwise, it implicitly contributes 0 (since we don't add
          // anything).
          if (pixelX >= 0 && pixelX < width && pixelY >= 0 && pixelY < height) {
            // Read input pixel value and multiply by corresponding kernel
            // weight Input image is stored in row-major order: index = y *
            // width + x Kernel is stored in row-major order: index = ky *
            // kernelSize + kx
            sum +=
                input[pixelY * width + pixelX] * kernel[ky * kernelSize + kx];
          }
        }
      }

      // Clamp results to 0-255 range to fit into uint8_t
      // If the sum is negative, clamp to 0 (black).
      if (sum < 0)
        sum = 0;
      // If the sum exceeds 255, clamp to 255 (white).
      if (sum > 255)
        sum = 255;

      // Store the result in the output buffer
      output[y * width + x] = static_cast<uint8_t>(sum);
    }
  }
}

void convolve2DSeparable(const uint8_t *input, uint8_t *output, int width,
                         int height, const int *kernelX, int kernelSizeX,
                         const int *kernelY, int kernelSizeY) {
  // Temporary buffer to store the result of the horizontal pass.
  // We use int to prevent overflow before the final clamping.
  std::vector<int> tempBuffer(width * height);

  int kHalfX = kernelSizeX / 2;
  int kHalfY = kernelSizeY / 2;

  // Pass 1: Horizontal Convolution
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int sum = 0;
      for (int kx = 0; kx < kernelSizeX; ++kx) {
        int pixelX = x + (kx - kHalfX);
        int pixelY = y; // Same row

        if (pixelX >= 0 && pixelX < width) {
          sum += input[pixelY * width + pixelX] * kernelX[kx];
        }
      }
      tempBuffer[y * width + x] = sum;
    }
  }

  // Pass 2: Vertical Convolution (on tempBuffer)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int sum = 0;
      for (int ky = 0; ky < kernelSizeY; ++ky) {
        int pixelX = x; // Same column
        int pixelY = y + (ky - kHalfY);

        if (pixelY >= 0 && pixelY < height) {
          sum += tempBuffer[pixelY * width + pixelX] * kernelY[ky];
        }
      }

      // Clamp results
      if (sum < 0)
        sum = 0;
      if (sum > 255)
        sum = 255;

      output[y * width + x] = static_cast<uint8_t>(sum);
    }
  }
}
