#include "placeholder.h"
#include <iostream>

void test_my_memmove();
void test_aligned_malloc();
void test_intrusive_list();
void test_find_first_set_bit();
void test_modify_reg();

int main() {
  std::cout << "Running tests..." << std::endl;
  placeholder();
  test_my_memmove();
  test_aligned_malloc();
  test_intrusive_list();
  test_find_first_set_bit();
  test_modify_reg();
  // Future tests will be called here
  return 0;
}
