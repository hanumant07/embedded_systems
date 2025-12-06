#include "intrusive_list.h"
#include <cassert>
#include <iostream>

// Helper to access the container structure from the node pointer
#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)))

// Utility to initialize a sentinel head
void list_init(struct ListNode *head) {
  head->next = head;
  head->prev = head;
}

void test_intrusive_list() {
  std::cout << "Testing intrusive_list..." << std::endl;

  // 1. **Initialization**
  ListNode head;
  list_init(&head);
  assert(head.next == &head);
  assert(head.prev == &head);
  std::cout << "  [PASS] Initialization" << std::endl;

  // 2. **Simple Append**
  Process p1 = {101, {nullptr, nullptr}};
  list_append(&head, &p1.node);

  assert(head.next == &p1.node);
  assert(head.prev == &p1.node);
  assert(p1.node.next == &head);
  assert(p1.node.prev == &head);
  std::cout << "  [PASS] Single Append" << std::endl;

  // 3. **Multiple Appends**
  Process p2 = {102, {nullptr, nullptr}};
  Process p3 = {103, {nullptr, nullptr}};

  list_append(&head, &p2.node);
  list_append(&head, &p3.node);

  // Verify order: head -> p1 -> p2 -> p3 -> head
  assert(head.next == &p1.node);
  assert(p1.node.next == &p2.node);
  assert(p2.node.next == &p3.node);
  assert(p3.node.next == &head);

  // Verify backward: head -> p3 -> p2 -> p1 -> head
  assert(head.prev == &p3.node);
  assert(p3.node.prev == &p2.node);
  assert(p2.node.prev == &p1.node);
  assert(p1.node.prev == &head);
  std::cout << "  [PASS] Multiple Appends" << std::endl;

  // 4. **Removal (Middle)**
  // Remove p2. List: p1 -> p3
  list_remove(&p2.node);

  assert(p1.node.next == &p3.node);
  assert(p3.node.prev == &p1.node);
  assert(p2.node.next == nullptr); // Verify detached
  std::cout << "  [PASS] Removal (Middle)" << std::endl;

  // 5. **Removal (Head/Tail)**
  // Remove p1 (first). List: p3
  list_remove(&p1.node);
  assert(head.next == &p3.node);
  assert(p3.node.prev == &head);

  // Remove p3 (last). List: empty
  list_remove(&p3.node);
  assert(head.next == &head);
  assert(head.prev == &head);
  std::cout << "  [PASS] Removal (Head/Tail)" << std::endl;

  std::cout << "intrusive_list tests passed!" << std::endl;
}
