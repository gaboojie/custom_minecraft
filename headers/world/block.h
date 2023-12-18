#ifndef MINECRAFT_HEADERS_WORLD_BLOCK_H
#define MINECRAFT_HEADERS_WORLD_BLOCK_H

#include <stdbool.h>
#include <shader.h>

struct BlockType {
  unsigned char transparency;
  unsigned int texID[6];
  unsigned char id;
  char* name;
  unsigned char nameLength;
};

void createBlockTypes();
void loadBlockTypes(struct ShaderProgram* program);
struct BlockType* createSameFaceBlockType(unsigned char id, unsigned char transparency, unsigned int texID, char* name, unsigned char nameLength);
struct BlockType* createBlockType(unsigned char id, unsigned char transparency, unsigned int* texID, char* name, unsigned char nameLength);

void deleteBlockTypes();

unsigned char isBlockTransparent(unsigned char blockType);
unsigned char isBlockAir(unsigned char blockType);
unsigned char getBlockType(unsigned short int blockData);
char* getBlockTypeName(unsigned char blockType);
unsigned char getBlockTypeNameLength(unsigned char blockType);

#endif
