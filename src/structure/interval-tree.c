#include <stdlib.h>
#include <stdio.h>
#include "interval-tree.h"
#include "math.h"

struct IntervalTreeNode* setBlockInIntervalTree(struct IntervalTreeNode* root, unsigned short int at, unsigned short int blockData) {
    struct IntervalTreeNode* on = getIntervalInTree(root, at);
    if (blockData == on->blockData) return root;
    if (on->low < at) {
      if (on->high > at) {
        // At is in the middle of an interval (not on the edges)
        unsigned short prevHigh = on->high;
        on->high = at-1;
        root = addIntervalInTree(root, at, at, blockData);
        root = addIntervalInTree(root, at+1, prevHigh, on->blockData);
      } else {
        // on is at top of interval node
        if (at != hashToIntervalTree(15, 15, 15)) {
          struct IntervalTreeNode* next = getIntervalInTree(root, at+1);
          if (next->blockData == blockData) {
            // On is at the top of interval node and the next interval has same block data, merge
            next->low = at;
            on->high = at-1;
          } else {
            // On is at the top of interval node and the next interval has different block data, do not merge
            on->high = at-1;
            root = addIntervalInTree(root, at, at, blockData);
          }
        } else {
          // On is at the top of interval node and is the highest hash
          on->high = at-1;
          root = addIntervalInTree(root, at, at, blockData);
        }
      }
    } else {
      if (on->high > at) {
        // on is at bottom of interval node
        if (at != 0) {
          // On is at bottom of
          struct IntervalTreeNode* prev = getIntervalInTree(root, at-1);
          if (prev->blockData == blockData) {
            // On is at bottom of interval node and prev interval has same block data, merge
            prev->high = at;
            on->low = at+1;
          } else {
            // On is at bottom of interval node and prev interval has different block data, do not merge
            on->low = at+1;
            root = addIntervalInTree(root, at, at, blockData);
          }
        } else {
          // On is at bottom of interval node and is the lowest hash
          on->low = at+1;
          root = addIntervalInTree(root, at, at, blockData);
        }
      } else {
        // on is the only node within interval
        if (at == 0) {
          struct IntervalTreeNode* next = getIntervalInTree(root, at+1);
          if (next->blockData == on->blockData) {
            // On is the lowest hash and has same block data as next, merge
            root = removeIntervalInTree(root, 0);
            next->low = 0;
          } else {
            // On is the lowest hash and has different block data as next, do not merge
            on->blockData = blockData;
          }
        } else if (at == hashToIntervalTree(15, 15, 15)) {
          struct IntervalTreeNode* prev = getIntervalInTree(root, at-1);
          if (prev->blockData == on->blockData) {
            // On is the highest hash and has same block data as prev, merge
            root = removeIntervalInTree(root, at);
            prev->high = at;
          } else {
            // On is the highest hash and has different block data as prev, do not merge
            on->blockData = blockData;
          }
        } else {
          struct IntervalTreeNode* prev = getIntervalInTree(root, at-1);
          struct IntervalTreeNode* next = getIntervalInTree(root, at+1);
          if (prev->blockData == blockData) {
            if (next->blockData == blockData) {
              // On can be merged with bottom and top intervals
              unsigned short high = next->high;
              root = removeIntervalInTree(root, at+1);
              root = removeIntervalInTree(root, at);

              struct IntervalTreeNode* newPrev = getIntervalInTree(root, at-1);
              newPrev->high = high;
            } else {
              // On can be merged with lower interval
              root = removeIntervalInTree(root, at);
              struct IntervalTreeNode* newPrev = getIntervalInTree(root, at-1);
              newPrev->high = at;
            }
          } else {
            if (next->blockData == blockData) {
              // On can be merged with higher interval
              root = removeIntervalInTree(root, at);
              struct IntervalTreeNode* newNext = getIntervalInTree(root, at+1);
              newNext->low = at;
            } else {
              // On cannot be merged
              on->blockData = blockData;
            }
          }
        }
      }
    }

    return root;
}

struct IntervalTreeNode* addIntervalInTree(struct IntervalTreeNode* root, unsigned short int from, unsigned short int to, unsigned short int blockData) {
    if (!root) return createIntervalTreeNode(from, to, blockData);

    if (from < root->low) {
        root->left = addIntervalInTree(root->left, from, to, blockData);
    } else {
        root->right = addIntervalInTree(root->right, from, to, blockData);
    }
    root->height = 1 + ((getIntervalNodeHeight(root->left) > getIntervalNodeHeight(root->right)) ? getIntervalNodeHeight(root->left) : getIntervalNodeHeight(root->right));
    char balance = getIntervalNodeBalance(root);

    if (balance > 1 && from < root->left->low) {
        return rotateRightIntervalTree(root);
    }
    if (balance < -1 && from > root->right->low) {
        return rotateLeftIntervalTree(root);
    }
    if (balance > 1 && from > root->left->low) {
        root->left = rotateLeftIntervalTree(root->left);
        return rotateRightIntervalTree(root);
    }
    if (balance < -1 && from < root->right->low) {
        root->right = rotateRightIntervalTree(root->right);
        return rotateLeftIntervalTree(root);
    }
    return root;
}

struct IntervalTreeNode* removeIntervalInTree(struct IntervalTreeNode* root, unsigned short int at) {
    if (!root) {
      return root;
    }
    if (at < root->low) {
      root->left = removeIntervalInTree(root->left, at);
    } else if (at > root->high) {
      root->right = removeIntervalInTree(root->right, at);
    } else {
      if ((!root->left) || (!root->right)) {
          struct IntervalTreeNode* temp = root->left ? root->left : root->right;

          if (!temp) {
            temp = root;
            root = NULL;
          } else {
            *root = *temp;
          }
          free(temp);
      } else {
          struct IntervalTreeNode* temp = getMinValueIntervalTree(root->right);
          root->low = temp->low;
          root->high = temp->high;
          root->blockData = temp->blockData;
          root->right = removeIntervalInTree(root->right, temp->low);
      }
    }
    if (!root) return root;

    root->height = 1 + ((getIntervalNodeHeight(root->left) > getIntervalNodeHeight(root->right)) ? getIntervalNodeHeight(root->left) : getIntervalNodeHeight(root->right));
    char balance = getIntervalNodeBalance(root);

    if (balance > 1) {
      if (getIntervalNodeBalance(root->left) >= 0) {
        return rotateRightIntervalTree(root);
      } else {
        root->left = rotateLeftIntervalTree(root->left);
        return rotateRightIntervalTree(root);
      }
    }
    if (balance < -1) {
      if (getIntervalNodeBalance(root->right) <= 0) {
        return rotateLeftIntervalTree(root);
      } else {
        root->right = rotateRightIntervalTree(root->right);
        return rotateLeftIntervalTree(root);
      }
    }


    return root;
}

struct IntervalTreeNode* getIntervalInTree(struct IntervalTreeNode* root, unsigned short int at) {
  while (root) {
    if (root->low <= at && root->high >= at) {
      return root;
    } else if (root->low > at) {
      root = root->left;
    } else if (root->high < at) {
      root = root->right;
    } else {
      break;
    }
  }
  printIntervalTreeRec(root, 0);
  printf("ERROR BLOCK IN CHUNK INTERVAL TREE D.N.E\n");
  printf("xyz: %u %u %u\n", getXFromHashIntervalTree(at), getYFromHashIntervalTree(at), getZFromHashIntervalTree(at));
  printf("at: %hu\n", at);
  exit(1);
  return NULL;
}

unsigned short int getIntervalBlockDataInTree(struct IntervalTreeNode* root, unsigned short int at) {
    while (root) {
          if (root->low <= at && root->high >= at) {
              return root->blockData;
          } else if (root->low > at) {
              root = root->left;
          } else if (root->high < at) {
              root = root->right;
          } else {
              break;
          }
    }
    printIntervalTreeRec(root, 0);
    printf("ERROR BLOCK IN CHUNK INTERVAL TREE D.N.E\n");
    printf("xyz: %u %u %u\n", getXFromHashIntervalTree(at), getYFromHashIntervalTree(at), getZFromHashIntervalTree(at));
    printf("at: %hu\n", at);
    exit(1);
    return 0xFF;
}

//
// Helper functions
//

unsigned short int hashToIntervalTree(unsigned char x, unsigned char y, unsigned char z) {
  return ((unsigned short int) x) + (((unsigned short int) z) << SHIFTS_PER_DIMENSION) + (((unsigned short int) y) << (SHIFTS_PER_DIMENSION * 2));
}
unsigned char getXFromHashIntervalTree(unsigned short int hash) {
  return (unsigned char) (hash & X_MASK);
}
unsigned char getYFromHashIntervalTree(unsigned short int hash) {
  return (unsigned char) ((hash & Y_MASK) >> (SHIFTS_PER_DIMENSION * 2));
}
unsigned char getZFromHashIntervalTree(unsigned short int hash) {
  return (unsigned char) ((hash & Z_MASK) >> (SHIFTS_PER_DIMENSION));
}

struct IntervalTreeNode* rotateLeftIntervalTree(struct IntervalTreeNode* node) {
  struct IntervalTreeNode* right = node->right;
  struct IntervalTreeNode* rightLeft = right->left;
  right->left = node;
  node->right = rightLeft;

  node->height = 1 + ((getIntervalNodeHeight(node->left) > getIntervalNodeHeight(node->right)) ? getIntervalNodeHeight(node->left) : getIntervalNodeHeight(node->right));
  right->height = 1 + ((getIntervalNodeHeight(right->left) > getIntervalNodeHeight(right->right)) ? getIntervalNodeHeight(right->left) : getIntervalNodeHeight(right->right));
  return right;
}
struct IntervalTreeNode* rotateRightIntervalTree(struct IntervalTreeNode* node) {
  struct IntervalTreeNode* left = node->left;
  struct IntervalTreeNode* leftRight = left->right;
  left->right = node;
  node->left = leftRight;

  node->height = 1 + ((getIntervalNodeHeight(node->left) > getIntervalNodeHeight(node->right)) ? getIntervalNodeHeight(node->left) : getIntervalNodeHeight(node->right));
  left->height = 1 + ((getIntervalNodeHeight(left->left) > getIntervalNodeHeight(left->right)) ? getIntervalNodeHeight(left->left) : getIntervalNodeHeight(left->right));
  return left;
}

struct IntervalTreeNode* getMinValueIntervalTree(struct IntervalTreeNode* root) {
  struct IntervalTreeNode* current = root;
  while (current->left) {
    current = current->left;
  }
  return current;
}

char getIntervalNodeHeight(struct IntervalTreeNode* node) {
  return (node ? node->height : 0);
}

char getIntervalNodeBalance(struct IntervalTreeNode* node) {
  return (node ? (getIntervalNodeHeight(node->left) - getIntervalNodeHeight(node->right)) : 0);
}

void printIntervalTreeRec(struct IntervalTreeNode* root, char currentTreeHeight) {
    if (root->left) printIntervalTreeRec(root->left, currentTreeHeight+1);
    printf("[%hu, %hu] - blockData: %hu at tree height: %hhx\n", root->low, root->high, root->blockData, currentTreeHeight);
    if (root->right) printIntervalTreeRec(root->right, currentTreeHeight+1);
}

struct IntervalTreeNode* createIntervalTreeNode(int low, int high, unsigned short int blockData) {
    struct IntervalTreeNode* node = (struct IntervalTreeNode*) malloc(sizeof(struct IntervalTreeNode));
    node->low = low;
    node->high = high;
    node->blockData = blockData;
    return node;
}

unsigned int getSizeOfIntervalTree(struct IntervalTreeNode* root) {
  if (root == NULL) return 0;

  unsigned int left = getSizeOfIntervalTree(root->left);
  unsigned int right = getSizeOfIntervalTree(root->right);

  return left + right + 1;
}

void deleteTreeNodes(struct IntervalTreeNode* root) {
    if (root->left) deleteTreeNodes(root->left);
    if (root->right) deleteTreeNodes(root->right);
    free(root);
}
