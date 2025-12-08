#ifndef MODIFY_REG_H
#define MODIFY_REG_H

#include <stdint.h> // for uint32_t

enum Op { SET, CLEAR, TOGGLE };

/**
 * @brief Modifies specific bits in a memory-mapped hardware register.
 *
 * @param reg_addr Pointer to the register. Marked volatile to prevent compiler
 * optimization.
 * @param mask Bitmask indicating which bits to modify.
 * @param operation The operation to perform (SET, CLEAR, TOGGLE).
 */
void modify_reg(volatile uint32_t *reg_addr, uint32_t mask, Op operation);

#endif // MODIFY_REG_H
