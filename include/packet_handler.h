#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>

// Exposed for testing/linking purposes.
// In a real scenario, this might be a callback or defined elsewhere.
void create_file(const char *name, int permissions);

// The main handler function to be implemented.
void receiver_handler(uint32_t packet);

#endif // PACKET_HANDLER_H
