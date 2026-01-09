#include "convolve2d.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

void test_convolve2d() {
  std::cout << "Testing convolve2d..." << std::endl;

  // Test Case 1: Identity Kernel
  {
    int width = 3;
    int height = 3;
    std::vector<uint8_t> input = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::vector<uint8_t> output(width * height);

    int kernelSize = 3;
    int kernel[] = {0, 0, 0, 0, 1, 0, 0, 0, 0};

    convolve2D(input.data(), output.data(), width, height, kernel, kernelSize);

    for (int i = 0; i < width * height; ++i) {
      assert(output[i] == input[i]);
    }
  }

  // Test Case 2: Simple Blur (All 1s kernel)
  {
    int width = 3;
    int height = 3;
    std::vector<uint8_t> input(width * height, 10); // All pixels 10
    std::vector<uint8_t> output(width * height);

    int kernelSize = 3;
    int kernel[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    convolve2D(input.data(), output.data(), width, height, kernel, kernelSize);

    // Center pixel should be 10 * 9 = 90
    assert(output[4] == 90);

    // Corner pixel (0,0) has 4 neighbors (including itself) in bounds due to
    // zero padding 10 * 4 = 40
    assert(output[0] == 40);
  }

  std::cout << "convolve2d tests passed!" << std::endl;

  std::cout << "Testing convolve2DSeparable..." << std::endl;
  // Test Case 3: Separable Blur (Box Blur)
  // A 3x3 kernel of all 1s can be separated into a 1x3 kernel of 1s and a 3x1
  // kernel of 1s.
  {
    int width = 3;
    int height = 3;
    std::vector<uint8_t> input(width * height, 10); // All pixels 10
    std::vector<uint8_t> output(width * height);

    int kernelSizeX = 3;
    int kernelX[] = {1, 1, 1};

    int kernelSizeY = 3;
    int kernelY[] = {1, 1, 1};

    convolve2DSeparable(input.data(), output.data(), width, height, kernelX,
                        kernelSizeX, kernelY, kernelSizeY);

    // Center pixel should be 10 * 3 * 3 = 90
    assert(output[4] == 90);

    // Corner pixel (0,0) has 2 neighbors in X and 2 in Y due to zero padding
    // 10 * 2 * 2 = 40 (Same result as original 2D convolution)
    assert(output[0] == 40);
  }

  std::cout << "convolve2DSeparable tests passed!" << std::endl;
}
