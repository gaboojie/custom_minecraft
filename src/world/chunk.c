#include "chunk.h"
#include "world-generator.h"
#include "model.h"

struct Chunk* createChunkFrom(struct IntervalTreeNode* blocks, int chunkX, int chunkY, int chunkZ) {
  struct Chunk* chunk = (struct Chunk*) malloc(sizeof (struct Chunk));
  chunk->blocks = blocks;
  chunk->chunkX = chunkX;
  chunk->chunkY = chunkY;
  chunk->chunkZ = chunkZ;
  chunk->model = generateModelFor(chunk);
  return chunk;
}

unsigned short int getBlockInChunkAt(struct Chunk* chunk, unsigned char x, unsigned char y, unsigned char z) {
    return getIntervalBlockDataInTree(chunk->blocks, hashToIntervalTree(x, y, z));
}

unsigned char isWithinChunk(unsigned char x, unsigned char y, unsigned char z) {
  return (0 <= x && x < 16 && 0 <= y && y < 16 && 0 <= z && z < 16);
}

void deleteChunk(struct Chunk* chunk) {
  deleteTreeNodes(chunk->blocks);
  deleteModel(chunk->model);
  free(chunk);
}
