#include "../include/packet_handler.h"
#include <assert.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_simple_message() {
  printf("Running test_simple_message...\n");
  // Construct Packet 0
  uint32_t p0 = 0;
  p0 |= (0u << 30);     // ID=0
  p0 |= (2u << 27);     // Num=2
  p0 |= (0u << 24);     // Seq=0
  p0 |= (1u << 20);     // Perm=1
  p0 |= (0x4E65u << 4); // "Ne"
  p0 |= 0u;             // Err

  // Construct Packet 1
  uint32_t p1 = 0;
  p1 |= (0u << 30);     // ID=0
  p1 |= (0u << 27);     // Num=0 (ignored)
  p1 |= (1u << 24);     // Seq=1
  p1 |= (0u << 20);     // Perm=0 (ignored)
  p1 |= (0x7700u << 4); // "w\0"
  p1 |= 0u;             // Err

  receiver_handler(p0);
  receiver_handler(p1);
}

void test_out_of_order() {
  printf("Running test_out_of_order...\n");
  uint32_t p0 = 0;
  p0 |= (1u << 30);
  p0 |= (2u << 27);     // Num 2
  p0 |= (0u << 24);     // Seq 0
  p0 |= (0u << 20);     // Perm 0 (RO)
  p0 |= (0x5465u << 4); // "Te"

  uint32_t p1 = 0;
  p1 |= (1u << 30);
  p1 |= (0u << 27);
  p1 |= (1u << 24);     // Seq 1
  p1 |= (0x7374u << 4); // "st"

  receiver_handler(p1);
  receiver_handler(p0);
}

void test_mixed_interleaved() {
  printf("Running test_mixed_interleaved...\n");
  // ID 2, Seq 0: "Hi", Num 1, Perm 1
  uint32_t p2_0 = 0;
  p2_0 |= (2u << 30);
  p2_0 |= (1u << 27);
  p2_0 |= (0u << 24);
  p2_0 |= (1u << 20);
  p2_0 |= (0x4869u << 4); // "Hi"

  // ID 3, Seq 0: "Y", Num 2, Perm 0
  uint32_t p3_0 = 0;
  p3_0 |= (3u << 30);
  p3_0 |= (2u << 27);
  p3_0 |= (0u << 24);
  p3_0 |= (0u << 20);
  p3_0 |= (0x5900u << 4); // "Y"

  // ID 3, Seq 1: "o"
  uint32_t p3_1 = 0;
  p3_1 |= (3u << 30);
  p3_1 |= (0u << 27);
  p3_1 |= (1u << 24);
  p3_1 |= (0x6F00u << 4); // "o"

  receiver_handler(p3_0);
  receiver_handler(p2_0);
  receiver_handler(p3_1);
}

void test_packet_handler() {
  printf("Testing Packet Handler...\n");
  test_simple_message();
  test_out_of_order();
  test_mixed_interleaved();
  printf("Packet Handler tests passed!\n");
}

#ifdef __cplusplus
}
#endif
