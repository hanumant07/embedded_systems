#include "connected_components.h"
#include <string.h>

// What is a Connected Component?
// In a binary image (where pixels are typically 0 for background and 1 for
// foreground), a "connected component" is a set of foreground pixels that are
// connected to each other. Connectivity is usually defined by adjacency:
// - 4-connectivity: Pixels are connected if they share an edge (horizontal or
// vertical neighbors).
// - 8-connectivity: Pixels are connected if they share an edge or a corner
// (horizontal, vertical, or diagonal neighbors).
//
// The goal of Connected Components Labeling (CCL) is to assign a unique integer
// label to each distinct connected component in the image. All pixels belonging
// to the same component get the same label. Background pixels typically get a
// label of 0.

int dfs(int x, int y, const uint8_t *input, int *labels, int width, int height,
        int label) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return 0;
  }

  if (input[y * width + x] == 0) {
    return 0;
  }

  if (labels[y * width + x] != 0) {
    return 0;
  }

  labels[y * width + x] = label;

  dfs(x + 1, y, input, labels, width, height, label);
  dfs(x - 1, y, input, labels, width, height, label);
  dfs(x, y + 1, input, labels, width, height, label);
  dfs(x, y - 1, input, labels, width, height, label);

  return 1;
}

/**
 * @brief Finds connected components in a binary image.
 *
 * @param input  Pointer to the input image data (row-major).
 *               This is a single byte per pixel (uint8_t). Non-zero values are
 * considered foreground/objects. Zero values are considered background.
 * @param labels Pointer to the output labels array (row-major).
 *               This buffer must be pre-allocated by the caller and have the
 * same number of elements as the input. Each pixel will be assigned an integer
 * label.
 * @param width  The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @return       The total number of connected components found.
 */
int findConnectedComponents(const uint8_t *input, int *labels, int width,
                            int height) {
  memset(labels, 0, width * height * sizeof(int));

  int label = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (input[y * width + x] == 0) {
        continue;
      }
      if (labels[y * width + x] != 0) {
        continue;
      }
      label++;
      dfs(x, y, input, labels, width, height, label);
    }
  }

  return label;
}
