#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct ListNode {
  struct ListNode *prev;
  struct ListNode *next;
};

// Example struct to demonstrate usage (Process)
// In a real intrusive list, the user would embed ListNode into their own
// structs.
struct Process {
  int pid;
  struct ListNode node; // Embedded node
};

/**
 * @brief Appends a new node to the end of the list.
 *
 * @param head Pointer to the head of the list (sentinel or actual head,
 * depending on implementation choice). For this interview context, assuming
 * simple circular or sentinel-based list logic might be expected, or just a
 * pointer to the first element. Clarification: "head" usually implies a handle
 * to the list.
 * @param new_node Pointer to the node to be appended.
 */
void list_append(struct ListNode *head, struct ListNode *new_node);

/**
 * @brief Removes a node from the list.
 *
 * @param node Pointer to the node to be removed.
 */
void list_remove(struct ListNode *node);

#ifdef __cplusplus
}
#endif

#endif // INTRUSIVE_LIST_H
