#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "block.h"

static struct BlockType** BLOCK_TYPES;
static const unsigned char NUM_BLOCK_TYPES = 11;

void createBlockTypes() {
  BLOCK_TYPES = malloc(sizeof(struct BlockType *) * NUM_BLOCK_TYPES);
  BLOCK_TYPES[0] = createSameFaceBlockType(0, 1, 0, "air", 0);
  BLOCK_TYPES[1] = createSameFaceBlockType(1, 0, 32*1+18, "dirt", 4);
  BLOCK_TYPES[2] = createSameFaceBlockType(2, 0, 32*1+5, "sand", 4);

  unsigned int grass_faces[] = { 2, 32*1+18, 3, 3, 3, 3};
  BLOCK_TYPES[3] = createBlockType(3, 0, grass_faces, "grass", 5);

  BLOCK_TYPES[4] = createSameFaceBlockType(4, 0, 31*1+0, "bedrock", 7);
  BLOCK_TYPES[5] = createSameFaceBlockType(5, 0, 32*0+19, "stone", 5);
  BLOCK_TYPES[6] = createSameFaceBlockType(6, 0, 1*32+17, "gravel", 6);
  unsigned int oak_wood_faces[] = {3*32+4, 3*32+4, 3*32+3, 3*32+3, 3*32+3, 3*32+3};
  BLOCK_TYPES[7] = createBlockType(7, 0, oak_wood_faces, "oak_wood", 8);

  BLOCK_TYPES[8] = createSameFaceBlockType(8, 1, 4*32+29, "oak_leaves", 10);
  BLOCK_TYPES[9] = createSameFaceBlockType(9, 1, 14*32+0, "water", 5);
  BLOCK_TYPES[10] = createSameFaceBlockType(10, 1, 14*32+2, "lava", 4);
}
void loadBlockTypes(struct ShaderProgram* program) {
  unsigned int* blockTexIDs = malloc(sizeof(unsigned int) * NUM_BLOCK_TYPES * 6);

  for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
      blockTexIDs[i*6] = BLOCK_TYPES[i]->texID[0];
      blockTexIDs[i*6+1] = BLOCK_TYPES[i]->texID[1];
      blockTexIDs[i*6+2] = BLOCK_TYPES[i]->texID[2];
      blockTexIDs[i*6+3] = BLOCK_TYPES[i]->texID[3];
      blockTexIDs[i*6+4] = BLOCK_TYPES[i]->texID[4];
      blockTexIDs[i*6+5] = BLOCK_TYPES[i]->texID[5];
  }
  bindProgram(program);
  loadBlockTexIDs(program, blockTexIDs, NUM_BLOCK_TYPES * 6);
  unbindProgram();
  free(blockTexIDs);
}

struct BlockType* createBlockType(unsigned char id, unsigned char transparency, unsigned int* texID, char* name, unsigned char nameLength) {
  struct BlockType* type = (struct BlockType *) malloc(sizeof(struct BlockType));
  type->id = id;
  type->transparency = transparency;
  type->texID[0] = texID[0];
  type->texID[1] = texID[1];
  type->texID[2] = texID[2];
  type->texID[3] = texID[3];
  type->texID[4] = texID[4];
  type->texID[5] = texID[5];
  type->name = (char *) malloc(sizeof(char) * (nameLength + 1));
  strcpy(type->name, name);
  return type;
}
struct BlockType* createSameFaceBlockType(unsigned char id, unsigned char transparency, unsigned int texID, char* name, unsigned char nameLength) {
  struct BlockType* type = (struct BlockType *) malloc(sizeof(struct BlockType));
  type->id = id;
  type->transparency = transparency;
  type->texID[0] = texID;
  type->texID[1] = texID;
  type->texID[2] = texID;
  type->texID[3] = texID;
  type->texID[4] = texID;
  type->texID[5] = texID;
  type->name = (char *) malloc(sizeof(char) * (nameLength + 1));
  strcpy(type->name, name);
  return type;
}

void deleteBlockTypes() {
    for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
        free(BLOCK_TYPES[i]->name);
        free(BLOCK_TYPES[i]);
    }
    free(BLOCK_TYPES);
}

//
// Helper functions
//

unsigned char isBlockTransparent(unsigned char blockType) {
  return (BLOCK_TYPES[blockType]->transparency) & 1;
}
unsigned char isBlockAir(unsigned char blockType) {
  return (blockType == 0) ? 1 : 0;
}
unsigned char getBlockType(unsigned short int blockData) {
  unsigned char c = (unsigned char) (blockData & 0xF);
  return c;
}
char* getBlockTypeName(unsigned char blockType) {
  return BLOCK_TYPES[blockType]->name;
}
unsigned char getBlockTypeNameLength(unsigned char blockType) {
  return BLOCK_TYPES[blockType]->nameLength;
}
