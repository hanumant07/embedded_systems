#include "packet_handler.h"
#include <cstdio>
#include <map>
#include <string>
#include <vector>

// Helper to print as per the interview prompt requirements (or mock for logic)
void create_file(const char *name, int permissions) {
  if (permissions == 0) {
    printf("Created read-only file of name %s\n", name);
  } else if (permissions == 1) {
    printf("Created read-write file of name %s\n", name);
  } else {
    printf("Unexpected permissions: %d\n", permissions);
  }
}

// Structure to hold state for a message being reassembled
struct MessageState {
  int num_packets = 0;
  int permissions = 0;
  std::vector<std::string> segments;
  int packets_received_count = 0;

  // Initialize segments vector with empty strings once we know num_packets
  void init(int n) {
    num_packets = n;
    segments.resize(n);
  }
};

// Global map to store partial messages by Message ID
static std::map<uint8_t, MessageState> message_store;

void receiver_handler(uint32_t packet) {
  // Extract fields
  uint8_t msg_id = (packet >> 30) & 0x03; // Bits 30-31
  uint8_t num_packets =
      (packet >> 27) & 0x07;               // Bits 27-29 (only in first packet?)
  uint8_t seq_num = (packet >> 24) & 0x07; // Bits 24-26
  uint8_t permissions =
      (packet >> 20) & 0x0F; // Bits 20-23 (only in first packet)
  uint16_t filename_seg = (packet >> 4) & 0xFFFF; // Bits 4-19
  // Error code (0-3) is unused for now based on description, or maybe we check
  // it? uint8_t error_code = packet & 0x0F;

  // Prompt says: "The bits for 'Num Packets' and 'Permissions' will only be set
  // in the first packet of each Message ID." BUT: "First packet" usually means
  // Sequence Num == 0. Let's assume Sequence Num 0 is the "header" packet
  // containing metadata.

  // Retrieve or create state for this Message ID
  MessageState &state = message_store[msg_id];

  // If this is the first sequence, set the metadata
  // Note: The problem description implies these fields are valid ONLY when
  // seq_num == 0? Or maybe "first packet" means the first one we receive?
  // Usually "first packet of each Message ID" implies the one with sequence
  // number 0.
  if (seq_num == 0) {
    // It's possible we receive Seq 0 *after* Seq 1 if out of order.
    // If we haven't initialized size yet, we should.
    // If we simply check if `state.num_packets == 0`, that works if we assume
    // num_packets > 0 always.

    // However, if we receive Seq 1 first, we might not know how big the vector
    // should be. We can resize dynamically or assume a max. The field is 3 bits
    // -> max 7 packets? Actually 3 bits = 0..7. If 0 is valid, max is 8? Or is
    // it 1-based? Let's assume it's the Total Count. The instructions say "Num
    // Packets... only set in the first packet".

    state.num_packets = num_packets;
    state.permissions = permissions;

    if (state.segments.size() < (size_t)num_packets) {
      state.segments.resize(num_packets);
    }
  }

  // Extract chars from filename segment
  // 16 bits = 2 chars.
  // Example: 0x4E65 -> 'N', 'e'.
  // 0x4E = 78 ('N'), 0x65 = 101 ('e')
  // So MSB is first char, LSB is second char.
  char c1 = (filename_seg >> 8) & 0xFF;
  char c2 = filename_seg & 0xFF;

  std::string segment_str;
  if (c1 != '\0')
    segment_str += c1;
  if (c2 != '\0')
    segment_str += c2;

  // Store segment
  // If we received out-of-order packets and we don't know num_packets yet
  // (because we missed seq 0), we need to handle resizing.
  if (state.segments.size() <= seq_num) {
    state.segments.resize(seq_num + 1);
  }

  if (state.segments[seq_num].empty() && !segment_str.empty()) {
    // Only count if this is a new packet for this slot ?
    // Or maybe we treat duplicate packets as no-op.
    // Simply counting received unique packets is safer.
    // But segment_str could be empty if it's just padding?
    // The example shows "New\0".
    // Packet 1: "Ne"
    // Packet 2: "w\0"
    // So segment_str is not empty.
    state.packets_received_count++;
  }
  // Overwrite is fine if duplicate
  state.segments[seq_num] = segment_str;

  // Check if complete
  // logic: if we have num_packets (from Seq 0) AND we have received that many
  // segments? Or just check if we have all slots filled? Since we might get Seq
  // 1 first, state.num_packets might be 0. We need to wait until we have seen
  // Seq 0.

  if (state.num_packets > 0 &&
      state.packets_received_count == state.num_packets) {
    // Reassemble
    std::string full_name;
    for (const auto &seg : state.segments) {
      full_name += seg;
    }

    // Call creator
    create_file(full_name.c_str(), state.permissions);

    // Cleanup?
    // "The handler function should process packets one at a time... as they
    // come in." Once a message is complete, we probably clear it so we can
    // reuse the ID? The image doesn't say IDs are unique forever. Reuse is
    // typical.
    message_store.erase(msg_id);
  }
}
