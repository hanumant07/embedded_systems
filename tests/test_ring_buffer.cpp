#include "ring_buffer.hpp"
#include <cassert>
#include <iostream>
#include <string>

struct CustomType {
  int id;
  std::string name;

  bool operator==(const CustomType &other) const {
    return id == other.id && name == other.name;
  }
};

void test_ring_buffer() {
  std::cout << "Testing RingBuffer<uint8_t>..." << std::endl;

  // Test initialization
  RingBuffer<uint8_t> rb(5);
  assert(rb.isEmpty());
  assert(!rb.isFull());

  // Test push
  assert(rb.push(1));
  assert(rb.push(2));
  assert(rb.push(3));
  assert(!rb.isEmpty());
  assert(!rb.isFull());

  // Test pop
  uint8_t val;
  assert(rb.pop(&val));
  assert(val == 1);
  assert(rb.pop(&val));
  assert(val == 2);

  // Test full capacity
  assert(rb.push(4));
  assert(rb.push(5));
  assert(rb.push(6));
  assert(rb.push(7));
  // Current state: 3, 4, 5, 6, 7.  (Capacity 5) -> Full

  assert(rb.isFull());
  assert(!rb.push(8)); // Should fail

  // Test circularity
  assert(rb.pop(&val));
  assert(val == 3);
  assert(!rb.isFull());
  assert(rb.push(8)); // Should succeed now

  std::cout << "RingBuffer<uint8_t> tests passed!" << std::endl;

  std::cout << "Testing RingBuffer<CustomType>..." << std::endl;
  RingBuffer<CustomType> rb_custom(3);

  CustomType a = {1, "A"};
  CustomType b = {2, "B"};
  CustomType c = {3, "C"};
  CustomType d = {4, "D"};

  assert(rb_custom.push(a));
  assert(rb_custom.push(b));
  assert(rb_custom.push(c));
  assert(rb_custom.isFull());
  assert(!rb_custom.push(d));

  CustomType out;
  assert(rb_custom.pop(&out));
  assert(out == a);
  assert(rb_custom.pop(&out));
  assert(out == b);
  assert(rb_custom.push(d));

  assert(rb_custom.pop(&out));
  assert(out == c);
  assert(rb_custom.pop(&out));
  assert(out == d);
  assert(rb_custom.isEmpty());

  std::cout << "RingBuffer<CustomType> tests passed!" << std::endl;
}
