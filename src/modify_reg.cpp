#include "modify_reg.h"

void modify_reg(volatile uint32_t *reg_addr, uint32_t mask, Op operation) {
  uint32_t val = *reg_addr; // Explicit Read
  switch (operation) {
  case SET:
    val |= mask;
    break;
  case CLEAR:
    val &= ~mask;
    break;
  case TOGGLE:
    val ^= mask;
    break;
  }
  *reg_addr = val; // Explicit Write
}
