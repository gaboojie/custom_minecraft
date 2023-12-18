#ifndef MINECRAFT_HEADERS_STRUCTURE_LINKED_LIST_H
#define MINECRAFT_HEADERS_STRUCTURE_LINKED_LIST_H

struct ListNode {
  void* data;
  struct ListNode* next;
};

struct ListNode* createListNode(void* data);
void deleteListNode(struct ListNode* node);
struct ListNode* addNodeToList(struct ListNode* root, struct ListNode* toAdd);
struct ListNode* getNodeAtListIndex(struct ListNode* root, unsigned int index);
struct ListNode* removeNodeAtListIndex(struct ListNode* root, unsigned int index);
unsigned int getListSize(struct ListNode* root);

#endif
