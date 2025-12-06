#include "intrusive_list.h"
#include <cstddef> // for NULL/nullptr

void list_append(struct ListNode *head, struct ListNode *new_node) {
  if (!head || !new_node)
    return;

  // Insert at the end (before head)
  struct ListNode *tail = head->prev;

  new_node->next = head;
  new_node->prev = tail;

  tail->next = new_node;
  head->prev = new_node;
}

void list_remove(struct ListNode *node) {
  if (!node || !node->prev || !node->next)
    return;

  node->prev->next = node->next;
  node->next->prev = node->prev;

  // Safe practice: clear pointers to indicate node is detached
  node->next = nullptr;
  node->prev = nullptr;
}
