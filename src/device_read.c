#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_REG_OFFSET 0x000
#define READ_REG_OFFSET 0x004
#define WRITE_REG_OFFSET 0x008
#define CTRL_REG_OFFSET 0x00C

#define STATUS_FIFO_FULL (1 << 0)
#define STATUS_FIFO_EMPTY (1 << 1)
#define STATUS_ERROR (0x3 << 2)
#define READ_DONE (1 << 4)
#define WRITE_DONE (1 << 5)
#define READ_MODE (1 << 6)
#define WRITE_MODE (1 << 7)

#define CTRL_READ_ENABLE (1 << 0)
#define CTRL_WRITE_ENABLE (1 << 1)
#define CTRL_RESET (1 << 2)

static uintptr_t DEVICE_BASE_ADDRESS = 0x1000;

void write_register(uintptr_t offset, uint32_t value) {
  volatile uint32_t *reg = (volatile uint32_t *)(DEVICE_BASE_ADDRESS + offset);
  *reg = value;
}

static inline uint32_t read_register(uintptr_t offset) {
  volatile uint32_t *reg = (volatile uint32_t *)(DEVICE_BASE_ADDRESS + offset);
  return *reg;
}

static inline void reset_device(uintptr_t base_address) {
  DEVICE_BASE_ADDRESS = base_address;
}

int read_from_device(size_t num_bytes, uint8_t *output_buffer,
                     size_t *read_bytes) {
  if (num_bytes <= 0 || !output_buffer || !read_bytes) {
    return -EINVAL;
  }

  write_register(CTRL_REG_OFFSET, CTRL_READ_ENABLE);

  volatile uint32_t status = 0;
  do {
    status = read_register(STATUS_REG_OFFSET);
  } while (status & READ_MODE);

  do {
    uint32_t read_val;
    size_t bytes_to_copy = sizeof(read_val);
    read_val = read_register(READ_REG_OFFSET);
    if (*read_bytes + bytes_to_copy > num_bytes) {
      bytes_to_copy = num_bytes - *read_bytes;
    }
    memcpy(output_buffer + *read_bytes, &read_val, bytes_to_copy);
    *read_bytes += bytes_to_copy;
    num_bytes -= bytes_to_copy;
    status = read_register(STATUS_REG_OFFSET);
    if (status & STATUS_ERROR) {
      return -EIO;
    }
    if (status & READ_DONE) {
      break;
    }
  } while (num_bytes > 0);

  return 0;
}

int write_to_device(size_t num_bytes, uint8_t *input_buffer,
                    size_t *written_bytes) {
  if (num_bytes <= 0 || !input_buffer || !written_bytes) {
    return -EINVAL;
  }

  write_register(CTRL_REG_OFFSET, CTRL_WRITE_ENABLE);

  volatile uint32_t status = 0;
  do {
    status = read_register(STATUS_REG_OFFSET);
  } while (status & WRITE_MODE);

  do {
    uint32_t write_val;
    size_t bytes_to_copy = sizeof(write_val);
    memcpy(&write_val, input_buffer + *written_bytes, bytes_to_copy);
    write_register(WRITE_REG_OFFSET, write_val);
    *written_bytes += bytes_to_copy;
    num_bytes -= bytes_to_copy;

    do {
      status = read_register(STATUS_REG_OFFSET);
    } while (status & STATUS_FIFO_FULL);

    if (status & STATUS_ERROR) {
      return -EIO;
    }
    if (status & WRITE_DONE) {
      break;
    }
  } while (num_bytes > 0);

  return 0;
}
