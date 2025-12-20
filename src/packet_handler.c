#include "packet_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_MSG_IDS 4
#define MAX_PACKETS 8
#define CHARS_PER_PACKET 2
#define MAX_FILENAME (MAX_PACKETS * CHARS_PER_PACKET + 1)

// Structure to hold state for a message being reassembled
typedef struct {
  int num_packets;
  int permissions;
  char segments[MAX_PACKETS][CHARS_PER_PACKET + 1];
  bool received[MAX_PACKETS];
  int packets_received_count;
  bool active;
} message_state_t;

// Global array to store partial messages by Message ID (0-3)
static message_state_t message_store[MAX_MSG_IDS];

void create_file(const char *name, int permissions) {
  if (permissions == 0) {
    printf("Created read-only file of name %s\n", name);
  } else if (permissions == 1) {
    printf("Created read-write file of name %s\n", name);
  } else {
    printf("Unexpected permissions: %d\n", permissions);
  }
}

void receiver_handler(uint32_t packet) {
  // Extract fields
  uint8_t msg_id = (packet >> 30) & 0x03;            // Bits 30-31
  uint8_t num_packets_field = (packet >> 27) & 0x07; // Bits 27-29
  uint8_t seq_num = (packet >> 24) & 0x07;           // Bits 24-26
  uint8_t permissions = (packet >> 20) & 0x0F;       // Bits 20-23
  uint16_t filename_seg = (packet >> 4) & 0xFFFF;    // Bits 4-19

  message_state_t *state = &message_store[msg_id];

  // If this is the "first" packet (seq 0) or we are initializing an inactive
  // slot
  if (!state->active) {
    memset(state, 0, sizeof(message_state_t));
    state->active = true;
  }

  if (seq_num == 0) {
    state->num_packets = num_packets_field;
    state->permissions = permissions;
  }

  // Extract chars from filename segment (2 chars per 16 bits)
  char c1 = (filename_seg >> 8) & 0xFF;
  char c2 = filename_seg & 0xFF;

  if (!state->received[seq_num]) {
    state->segments[seq_num][0] = c1;
    state->segments[seq_num][1] = c2;
    state->segments[seq_num][2] = '\0';
    state->received[seq_num] = true;
    state->packets_received_count++;
  }

  // Check if complete
  // We need num_packets (from seq 0) and all packets received
  if (state->num_packets > 0 &&
      state->packets_received_count == state->num_packets) {
    char full_name[MAX_FILENAME] = {0};
    for (int i = 0; i < state->num_packets; i++) {
      // Only append non-null characters
      if (state->segments[i][0] != '\0') {
        size_t len = strlen(full_name);
        full_name[len] = state->segments[i][0];
        full_name[len + 1] = '\0';
      }
      if (state->segments[i][1] != '\0') {
        size_t len = strlen(full_name);
        full_name[len] = state->segments[i][1];
        full_name[len + 1] = '\0';
      }
    }

    create_file(full_name, state->permissions);

    // Reset state for this ID
    state->active = false;
    state->num_packets = 0;
    state->packets_received_count = 0;
    memset(state->received, 0, sizeof(state->received));
  }
}
