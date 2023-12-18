#ifndef MINECRAFT_HEADERS_WORLD_CHUNK_H
#define MINECRAFT_HEADERS_WORLD_CHUNK_H

#include "interval-tree.h"
#include "model.h"

struct Chunk {
  struct IntervalTreeNode* blocks;
  struct Model* model;
  int chunkX, chunkY, chunkZ;
};

struct Chunk* createChunkFrom(struct IntervalTreeNode* blocks, int chunkX, int chunkY, int chunkZ);
unsigned short int getBlockInChunkAt(struct Chunk* chunk, unsigned char x, unsigned char y, unsigned char z);
unsigned char isWithinChunk(unsigned char x, unsigned char y, unsigned char z);
void deleteChunk(struct Chunk* chunk);

#endif
