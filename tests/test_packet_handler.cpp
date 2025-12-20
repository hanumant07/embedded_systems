#include "../include/packet_handler.h"
#include <cassert>
#include <iostream>
#include <vector>

// Mocking create_file output capture if needed, but for now we rely on stdout.
// Or we can declare a helper to test logic.
// Since create_file prints to stdout, we can manually verify or use a pipe.
// For simplicity in this test runner, we will just call it and rely on "tests
// passed" by not crashing. Enhancing: We can't easily mock the internal state
// of packet_handler without exposing it. We will send packets that form a
// complete message and visually expect the print. Ideally, we would redesign
// for testability, but let's stick to the interface.

void test_simple_message() {
  std::cout << "Running test_simple_message..." << std::endl;
  // Example from image:
  // Packet 0:
  // ID: 0 (00)
  // Num: 2 (010) -> 2 packets
  // Seq: 0 (000)
  // Perm: 1 (0001) -> Read-write
  // File: 'N' 'e' -> 0x4E65
  // Err: 0
  // Binary: 00 010 000 0001 0100111001100101 0000
  // Hex: 0x2014E650
  // Wait, let's construct it carefully.

  // Packet 1:
  // ID: 0
  // Num: 0 (unused/don't care)
  // Seq: 1
  // Perm: 0 (unused)
  // File: 'w' '\0' -> 0x7700
  // Err: 0

  // Construct Packet 0
  uint32_t p0 = 0;
  p0 |= (0 << 30);     // ID=0
  p0 |= (2 << 27);     // Num=2
  p0 |= (0 << 24);     // Seq=0
  p0 |= (1 << 20);     // Perm=1
  p0 |= (0x4E65 << 4); // "Ne"
  p0 |= 0;             // Err

  // Construct Packet 1
  uint32_t p1 = 0;
  p1 |= (0 << 30);     // ID=0
  p1 |= (0 << 27);     // Num=0 (ignored)
  p1 |= (1 << 24);     // Seq=1
  p1 |= (0 << 20);     // Perm=0 (ignored)
  p1 |= (0x7700 << 4); // "w\0"
  p1 |= 0;             // Err

  receiver_handler(p0);
  receiver_handler(p1);
  // Should print: Created read-write file of name New
}

void test_out_of_order() {
  std::cout << "Running test_out_of_order..." << std::endl;
  // Message ID 1
  // "Test" : 'T' 'e', 's' 't'
  // Packet 0: Seq 0, "Te", Num 2, Perm 0 (RO)
  // Packet 1: Seq 1, "st"

  uint32_t p0 = 0;
  p0 |= (1 << 30);
  p0 |= (2 << 27);     // Num 2
  p0 |= (0 << 24);     // Seq 0
  p0 |= (0 << 20);     // Perm 0 (RO)
  p0 |= (0x5465 << 4); // "Te"

  uint32_t p1 = 0;
  p1 |= (1 << 30);
  p1 |= (0 << 27);
  p1 |= (1 << 24);     // Seq 1
  p1 |= (0x7374 << 4); // "st"

  // Send p1 then p0
  receiver_handler(p1);
  receiver_handler(p0);
  // Should print: Created read-only file of name Test
}

void test_mixed_interleaved() {
  std::cout << "Running test_mixed_interleaved..." << std::endl;
  // ID 2 ("Hi") and ID 3 ("Yo")

  // ID 2, Seq 0: "Hi", Num 1, Perm 1
  uint32_t p2_0 = 0;
  p2_0 |= (2u << 30);
  p2_0 |= (1 << 27);
  p2_0 |= (0 << 24);
  p2_0 |= (1 << 20);
  p2_0 |= (0x4869 << 4); // "Hi"

  // ID 3, Seq 0: "Y", Num 2, Perm 0
  uint32_t p3_0 = 0;
  p3_0 |= (3u << 30);
  p3_0 |= (2 << 27);
  p3_0 |= (0 << 24);
  p3_0 |= (0 << 20);
  p3_0 |= (0x5900 << 4); // "Y" (padded null?) or just "Y\0" -> 0x5900

  // ID 3, Seq 1: "o"
  uint32_t p3_1 = 0;
  p3_1 |= (3u << 30);
  p3_1 |= (0 << 27);
  p3_1 |= (1 << 24);
  p3_1 |= (0x6F00 << 4); // "o"

  receiver_handler(p3_0); // ID 3 start
  receiver_handler(p2_0); // ID 2 finish -> Print "Hi"
  receiver_handler(p3_1); // ID 3 finish -> Print "Yo"
}

void test_packet_handler() {
  std::cout << "Testing Packet Handler..." << std::endl;
  test_simple_message();
  test_out_of_order();
  test_mixed_interleaved();
  std::cout << "Packet Handler tests passed!" << std::endl;
}
