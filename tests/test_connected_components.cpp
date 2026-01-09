#include "connected_components.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_connected_components() {
  std::cout << "Testing connected components..." << std::endl;

  // Test Case 1: Two separate components
  {
    int width = 4;
    int height = 4;
    std::vector<uint8_t> input = {1, 0, 0, 1, 1, 0, 0, 1,
                                  0, 0, 0, 0, 0, 1, 1, 0};
    // Expected:
    // Component 1: (0,0), (0,1)
    // Component 2: (3,0), (3,1)
    // Component 3: (1,3), (2,3)
    // Total 3 components.

    std::vector<int> labels(width * height);
    int count =
        findConnectedComponents(input.data(), labels.data(), width, height);

    // Debug output if failure expected
    if (count != 3) {
      std::cout << "Expected 3 components, got " << count << std::endl;
    }
    assert(count == 3);

    // Verify labeling (same component should have same label)
    assert(labels[0] == labels[4]);   // (0,0) and (0,1) connected
    assert(labels[3] == labels[7]);   // (3,0) and (3,1) connected
    assert(labels[13] == labels[14]); // (1,3) and (2,3) connected

    // Verify distinct labels
    assert(labels[0] != labels[3]);
    assert(labels[0] != labels[13]);
  }

  // Test Case 2: Empty image
  {
    int width = 3;
    int height = 3;
    std::vector<uint8_t> input(width * height, 0);
    std::vector<int> labels(width * height);
    int count =
        findConnectedComponents(input.data(), labels.data(), width, height);
    assert(count == 0);
  }

  // Test Case 3: Full image (one component)
  {
    int width = 3;
    int height = 3;
    std::vector<uint8_t> input(width * height, 1);
    std::vector<int> labels(width * height);
    int count =
        findConnectedComponents(input.data(), labels.data(), width, height);
    assert(count == 1);
  }

  std::cout << "connected_components tests passed!" << std::endl;
}
