#include <stdlib.h>
#include "linked-list.h"

struct ListNode* createListNode(void* data) {
  struct ListNode* node = malloc(sizeof(struct ListNode));
  node->data = data;
  node->next = NULL;
  return node;
}
void deleteListNode(struct ListNode* node) {
  free(node);
}
struct ListNode* addNodeToList(struct ListNode* root, struct ListNode* toAdd) {
  if (root == NULL) return toAdd;
  struct ListNode* on = root;
  while (on->next != NULL) {
    on = on->next;
  }
  on->next = toAdd;
  return root;
}
struct ListNode* getNodeAtListIndex(struct ListNode* root, unsigned int index) {
  if (root == NULL) return NULL;
  struct ListNode* on = root;
  unsigned int indexOn = 0;
  while (indexOn != index) {
    if (on->next == NULL) {
      return NULL;
    }
    on=on->next;
    indexOn++;
  }
  return on;
}
struct ListNode* removeNodeAtListIndex(struct ListNode* root, unsigned int index) {
  if (root == NULL) return NULL;
  struct ListNode* on = root;
  struct ListNode* prev = NULL;
  unsigned int indexOn = 0;
  while (indexOn != index) {
    if(on->next == NULL) {
      return root;
    }
    prev = on;
    on=on->next;
    indexOn++;
  }
  if (prev) {
    prev->next = on->next;
  }
  return on;
}
unsigned int getListSize(struct ListNode* root) {
  if (root == NULL) return 0;
  struct ListNode* on = root;
  int size = 1;
  while (on->next != NULL) {
    size++;
    on = on->next;
  }
  return size;
}
