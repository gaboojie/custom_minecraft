#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "world-generator.h"
#include "interval-tree.h"
#include "block.h"
#include "chunk.h"
#include "cglm/vec3.h"
#include "vao.h"

// I need: char consideredFaces[16*16*16]
// I need: a function to get if a face is considered using a block's x, y, z and face dir
// I need: a function to set if a face is considered using a block's x, y, z and face dir

// I need: float vertices[16*16*16*6*2*3] - 6 faces, 16 * 16 * 16 blocks, 2 triangles per face, 3 positions per vertex
// I need: float texCoords[16*16*16*6*2*2] - 6 faces, 16 * 16 * 16 blocks, 2 triangles per face, 2 texCoords per vertex
// I need: int vertexOn = 0
// I need: int texCoordOn = 0

unsigned char consideredFaces[16*16*16];
unsigned int data[16*16*16*6*2];

// 0x1111 1111 0000 0000 0000 0000 0000 0000
// Block Type

// 0x0000 0000 1111 1000 0000 0000 0000 0000
// Face direction

// 0x0000 0000 0000 0011 1111 1111 11 11 1111
// Compressed vertices
// Each vertex can go from 0->32, which takes 6 bits to represent (18 total)

unsigned int dataOn = 0;

const char UP_NORMAL[] = { 0, 1, 0}, DOWN_NORMAL[] = {0, -1, 0};
const char NEG_X_NORMAL[] = { -1, 0, 0}, POS_X_NORMAL[] = {1, 0, 0};
const char NEG_Z_NORMAL[] = {0, 0, -1}, POS_Z_NORMAL[] = {0, 0, 1};
const unsigned char UP_DIRECTION = 1, DOWN_DIRECTION = 2, NEG_X_DIRECTION = 4, POS_X_DIRECTION = 8, NEG_Z_DIRECTION = 16, POS_Z_DIRECTION = 32;

struct IntervalTreeNode* currentChunkTree;

struct Chunk* createChunk(int chunkX, int chunkY, int chunkZ) {
  struct IntervalTreeNode* blocks = addIntervalInTree(0, 0, hashToIntervalTree(15, 3, 15), 1);
  blocks = addIntervalInTree(blocks, hashToIntervalTree(0, 4, 0), hashToIntervalTree(15, 4, 15), 3);
  blocks = addIntervalInTree(blocks, hashToIntervalTree(0, 5, 0), hashToIntervalTree(15, 15, 15), 0);

  return createChunkFrom(blocks, chunkX, chunkY, chunkZ);
}

struct Model* generateModelFor(struct Chunk* chunk) {
  currentChunkTree = chunk->blocks;
  resetChunkGeneration();

  for (unsigned char x = 0; x < 16; x++) {
    for (unsigned char y = 0; y < 16; y++) {
      for (unsigned char z = 0; z < 16; z++) {
        processBlock(x, y, z, UP_NORMAL, 1, UP_DIRECTION);
        processBlock(x, y, z, DOWN_NORMAL, 0, DOWN_DIRECTION);
        processBlock(x, y, z, NEG_X_NORMAL, 0, NEG_X_DIRECTION);
        processBlock(x, y, z, POS_X_NORMAL, 1, POS_X_DIRECTION);
        processBlock(x, y, z, NEG_Z_NORMAL, 0, NEG_Z_DIRECTION);
        processBlock(x, y, z, POS_Z_NORMAL, 1, POS_Z_DIRECTION);
      }
    }
  }

//  for (int i = 0; i < (vertexOn / 3); i++) {
//    printf("(%f, %f, %f)\n", vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
//  }
  //printf("%hu %hu", vertexOn / 3, texCoordOn / 2);

  struct Transform* transform = createTransform((vec3) {chunk->chunkX * 16, chunk->chunkY * 16, chunk->chunkZ * 16}, (vec3) {0, 0, 0}, (vec3) {1, 1, 1});
  struct VAO* vao = createDataVAO(data, (int) dataOn);
  return createModel(transform, vao);
}

// Run functions
void processBlock(unsigned char x, unsigned char y, unsigned char z, const char* normal, char faceOffset, unsigned char faceDirection) {
  if (!isFaceConsidered(x, y, z, faceDirection)) {
    unsigned char blockType = getBlockType(getIntervalBlockDataInTree(currentChunkTree, hashToIntervalTree(x, y, z)));
    if (isBlockAir(blockType) || !isPossibleBlockTransparent(x + normal[0], y + normal[1], z + normal[2]) || (isSameType(blockType, x + normal[0], y + normal[1], z + normal[2]))) return;
    // if is same type and transparent for block at normal offset, don't make face

    unsigned char x2 = x;
    unsigned char y2 = y;
    unsigned char z2 = z;

    // Create runs in the directions that perpendicular to the normal
    if (!normal[0]) {
      x2 = startXRun(x, y, z, blockType, normal, faceDirection);
      if (!normal[1]) {
        y2 = continueWithXYRun(x, y, z, x2, blockType, normal, faceDirection);
        if (normal[2] == 1) {
          addFace(faceDirection,
                  (unsigned int[]) {x, y, z + faceOffset},
                  (unsigned int[]) {x2 + 1, y, z + faceOffset},
                  (unsigned int[]) {x2 + 1, y2 + 1, z2 + faceOffset},
                  (unsigned int[]) {x, y2 + 1, z2 + faceOffset},
                  blockType);
        } else {
          addFace(faceDirection,
                  (unsigned int[]) {x2 + 1, y, z + faceOffset},
                  (unsigned int[]) {x, y, z + faceOffset},
                  (unsigned int[]) {x, y2 + 1, z2 + faceOffset},
                  (unsigned int[]) {x2 + 1, y2 + 1, z2 + faceOffset},
                  blockType);
        }
      } else {
        z2 = continueWithXZRun(x, y, z, x2, blockType, normal, faceDirection);
        if (normal[1] == 1) {
          addFace(faceDirection,
                  (unsigned int[]) {x2 + 1, y + faceOffset, z},
                  (unsigned int[]) {x, y + faceOffset, z},
                  (unsigned int[]) {x, y + faceOffset, z2 + 1},
                  (unsigned int[]) {x2 + 1, y2 + faceOffset, z2 + 1},
                  blockType);
        } else {
          addFace(faceDirection,
                  (unsigned int[]) {x, y + faceOffset, z},
                  (unsigned int[]) {x2 + 1, y + faceOffset, z},
                  (unsigned int[]) {x2 + 1, y2 + faceOffset, z2 + 1},
                  (unsigned int[]) {x, y + faceOffset, z2 + 1},
                  blockType);
        }
      }
    } else {
      y2 = startYRun(x, y, z, blockType, normal, faceDirection);
      z2 = continueWithYZRun(x, y, z, y2, blockType, normal, faceDirection);
      if (normal[0] == 1) {
        addFace(faceDirection,
                (unsigned int[]) {x + faceOffset, y, z},
                (unsigned int[]) {x + faceOffset, y2 + 1, z},
                (unsigned int[]) {x2 + faceOffset, y2 + 1, z2 + 1},
                (unsigned int[]) {x + faceOffset, y, z2 + 1},
                blockType);
      } else {
        addFace(faceDirection,
                (unsigned int[]){x + faceOffset, y, z},
                (unsigned int[]) {x + faceOffset, y, z2 + 1},
                (unsigned int[]) {x2 + faceOffset, y2 + 1, z2 + 1},
                (unsigned int[]) {x + faceOffset, y2 + 1, z},
                blockType);
      }
    }

    for (unsigned char xOn = x; xOn <= x2; xOn++) {
      for (unsigned char yOn = y; yOn <= y2; yOn++) {
        for (unsigned char zOn = z; zOn <= z2; zOn++) {
          setFaceConsidered(xOn, yOn, zOn, faceDirection);
        }
      }
    }
  }
}

// Initiate X run
unsigned char startXRun(unsigned char x, unsigned char y, unsigned char z, unsigned char blockType, const char* normal, unsigned char faceDirection) {
  for (unsigned char offsetX = x + 1; offsetX < 16; offsetX++) {
    if(isSameType(blockType, offsetX, y, z) && isPossibleBlockTransparent(offsetX + normal[0], y+normal[1], z + normal[2]) && !isFaceConsidered(offsetX, y, z, faceDirection)) {
      x++;
    } else break;
  }
  return x;
}

// Initiate Y run
unsigned char startYRun(unsigned char x, unsigned char y, unsigned char z, unsigned char blockType, const char* normal, unsigned char faceDirection) {
  for (unsigned char offsetY = y + 1; offsetY < 16; offsetY++) {
    if(isSameType(blockType, x, offsetY, z) && isPossibleBlockTransparent(x + normal[0], offsetY+normal[1], z + normal[2]) && !isFaceConsidered(x, offsetY, z, faceDirection)) {
      y++;
    } else break;
  }
  return y;
}

// Continue XY run
unsigned char continueWithXYRun(unsigned char x, unsigned char y, unsigned char z, unsigned char x2, unsigned char blockType, const char* normal, unsigned char faceDirection) {
  for (unsigned char offsetY = y + 1; offsetY < 16; offsetY++) {
    unsigned char canExtend = 1;
    for (unsigned char offsetX = x; offsetX <= x2; offsetX++) {
      if (!isSameType(blockType, offsetX, offsetY, z) || !isPossibleBlockTransparent(offsetX + normal[0], offsetY + normal[1], z + normal[2]) || isFaceConsidered(offsetX, offsetY, z, faceDirection)) {
        canExtend = 0;
        break;
      }
    }
    if (canExtend) {
      y++;
    } else
      break;
  }
  return y;
}

// Continue YZ run
unsigned char continueWithYZRun(unsigned char x, unsigned char y, unsigned char z, unsigned char y2, unsigned char blockType, const char* normal, unsigned char faceDirection) {
  for (unsigned char offsetZ = z + 1; offsetZ < 16; offsetZ++) {
    unsigned char canExtend = 1;
    for (unsigned char offsetY = y; offsetY <= y2; offsetY++) {
      if (!isSameType(blockType, x, offsetY, offsetZ) || !isPossibleBlockTransparent(x + normal[0], offsetY + normal[1], offsetZ + normal[2]) || isFaceConsidered(x, offsetY, offsetZ, faceDirection)) {
        canExtend = 0;
        break;
      }
    }
    if (canExtend) {
      z++;
    } else
      break;
  }
  return z;
}

// Continue XZ run
unsigned char continueWithXZRun(unsigned char x, unsigned char y, unsigned char z, unsigned char x2, unsigned char blockType, const char* normal, unsigned char faceDirection) {
  for (unsigned char offsetZ = z + 1; offsetZ < 16; offsetZ++) {
    unsigned char canExtend = 1;
    for (unsigned char offsetX = x; offsetX <= x2; offsetX++) {
      if (!isSameType(blockType, offsetX, y, offsetZ) || !isPossibleBlockTransparent(offsetX + normal[0], y + normal[1], offsetZ + normal[2]) || isFaceConsidered(offsetX, y, offsetZ, faceDirection)) {
        canExtend = 0;
        break;
      }
    }
    if (canExtend) {
      z++;
    } else
      break;
  }
  return z;
}

//
// Helper functions
//

unsigned char isPossibleBlockTransparent(unsigned char x, unsigned char y, unsigned char z) {
  if (!isWithinChunk(x, y, z)) return 1;
  return isBlockTransparent(getBlockType(getIntervalBlockDataInTree(currentChunkTree, hashToIntervalTree(x, y, z))));
}

unsigned char isSameType(unsigned char blockData, unsigned char x, unsigned char y, unsigned char z) {
  if (!isWithinChunk(x, y, z)) return 0;
  return (getBlockType(getIntervalBlockDataInTree(currentChunkTree, hashToIntervalTree(x, y, z))) == blockData);
}

void addFace(unsigned char faceDirection, unsigned int v1[3], unsigned int v2[3], unsigned int v3[3], unsigned int v4[3], unsigned char blockType) {
  addVertex(v1, faceDirection, blockType);
  addVertex(v2, faceDirection, blockType);
  addVertex(v3, faceDirection, blockType);

  addVertex(v1, faceDirection, blockType);
  addVertex(v3, faceDirection, blockType);
  addVertex(v4, faceDirection, blockType);
}

void addVertex(unsigned int pos[3], unsigned char faceDirection, unsigned char blockType) {
  unsigned int vertexData = (pos[0] << 12) | (pos[1] << 6) | pos[2];
  data[dataOn] = (((unsigned int) blockType) << 24) | (((unsigned int) faceDirection) << 18) | vertexData;
  dataOn++;
}

unsigned int getFaceDirTextureOffset(unsigned char faceDirection) {
  switch (faceDirection) {
  case 1:
    return 0;
  case 2:
    return 1;
  case 4:
    return 2;
  case 8:
    return 3;
  case 16:
    return 4;
  case 32:
    return 5;
  default:
    return 0;
  }
}

void resetChunkGeneration() {
  memset(consideredFaces, 0, 16*16*16);
  dataOn = 0;
}

unsigned char isFaceConsidered(unsigned char x, unsigned char y, unsigned char z, unsigned char faceDirection) {
  return consideredFaces[hashToIntervalTree(x, y, z)] & faceDirection;
}
void setFaceConsidered(unsigned char x, unsigned char y, unsigned char z, unsigned char faceDirection) {
  consideredFaces[hashToIntervalTree(x, y, z)] = consideredFaces[hashToIntervalTree(x, y, z)] | faceDirection;
}
