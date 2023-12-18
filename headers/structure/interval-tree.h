#ifndef MINECRAFT_HEADERS_INTERVAL_TREE_H
#define MINECRAFT_HEADERS_INTERVAL_TREE_H

/* Example: [0-4: Grass, 5-9: Dirt, 10-15: Stone]
 *       5-9: Dirt
 *      /         \
 * 0-4: Grass     10-15: Stone
 */

// What methods do I want?
// I want to be able to set a block -> void setBlockInTree(treeNode* root, unsigned short int at, unsigned short int blockData)
// I want to be able to insert an interval -> void addIntervalInTree(treeNode* root, unsigned short int from, unsigned short int int to, unsigned short int blockData) (Initialization)
// I want to be able to get an interval -> unsigned short int getIntervalBlockDataInTree(treeNode* root, int at)
// HELPER METHOD: I want to be able to remove an interval -> void removeIntervalInTree(treeNode* root, int at)
// HELPER METHOD: I want to be able to print tree -> void printTreeNode(treeNode* root, int currentHeight)
// HELPER METHOD: Create interval node -> treeNode* createTreeNode(int low, int high, unsigned short int blockData)
// HELPER METHOD: Destroy interval node -> void destroyTreeNodes(treeNode* root)
// I want to be able to in-order traverse tree

static const unsigned short int SHIFTS_PER_DIMENSION = 4;
static const unsigned short int X_MASK = 0xF;
static const unsigned short int Z_MASK = X_MASK << SHIFTS_PER_DIMENSION;
static const unsigned short int Y_MASK = Z_MASK << SHIFTS_PER_DIMENSION;

struct IntervalTreeNode {
    // Tree requirements
    struct IntervalTreeNode* left;
    struct IntervalTreeNode* right;
    char height;

    // Value requirements
    unsigned short int blockData;
    unsigned short int low, high;
};

struct IntervalTreeNode* setBlockInIntervalTree(struct IntervalTreeNode* root, unsigned short int at, unsigned short int blockData);
struct IntervalTreeNode* addIntervalInTree(struct IntervalTreeNode* root, unsigned short int from, unsigned short int to, unsigned short int blockData);
struct IntervalTreeNode* removeIntervalInTree(struct IntervalTreeNode* root, unsigned short int at);
unsigned short int getIntervalBlockDataInTree(struct IntervalTreeNode* root, unsigned short int at);
struct IntervalTreeNode* getIntervalInTree(struct IntervalTreeNode* root, unsigned short int at);
void printIntervalTreeRec(struct IntervalTreeNode* root, char currentTreeHeight);
struct IntervalTreeNode* createIntervalTreeNode(int low, int high, unsigned short int blockData);
void deleteTreeNodes(struct IntervalTreeNode* root);
char getIntervalNodeHeight(struct IntervalTreeNode* node);
char getIntervalNodeBalance(struct IntervalTreeNode* node);
struct IntervalTreeNode* getMinValueIntervalTree(struct IntervalTreeNode* root);

unsigned short int hashToIntervalTree(unsigned char x, unsigned char y, unsigned char z);
unsigned char getXFromHashIntervalTree(unsigned short int hash);
unsigned char getYFromHashIntervalTree(unsigned short int hash);
unsigned char getZFromHashIntervalTree(unsigned short int hash);
unsigned int getSizeOfIntervalTree(struct IntervalTreeNode* root);


struct IntervalTreeNode* rotateLeftIntervalTree(struct IntervalTreeNode* node);
struct IntervalTreeNode* rotateRightIntervalTree(struct IntervalTreeNode* node);

#endif
