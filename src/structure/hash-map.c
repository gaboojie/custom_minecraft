#include <stdlib.h>
#include <stdio.h>
#include "hash-map.h"
#include "chunk.h"

struct HashMap* createHashMap(int size, float loadFactor) {
  struct HashMap *map = malloc(sizeof(struct HashMap));
  size = size * ((int) (1 / loadFactor));

  void **values = calloc(size, sizeof(void *));

  map->size = size;
  map->values = values;
  return map;
}

struct Chunk* getChunkAtKeyInMap(int x, int y, int z, struct HashMap* map) {
  unsigned int cellOn = getHashForIVec3(x, y, z) % map->size;
  struct Chunk* chunkOn = map->values[cellOn];

  // Get by sequential search, assuming load factor is < 0.7
  while (chunkOn != NULL) {
    if (chunkOn->chunkX == x && chunkOn->chunkY == y && chunkOn->chunkZ == z) {
      return chunkOn;
    }

    cellOn += 1;
    if (cellOn >= map->size) cellOn = 0;
    chunkOn = map->values[cellOn];
  }
  return 0;
}
void putChunkInMap(struct Chunk* chunk, struct HashMap* map) {
  unsigned int cellOn = getHashForIVec3(chunk->chunkX, chunk->chunkY, chunk->chunkZ) % map->size;
  struct Chunk* chunkOn = map->values[cellOn];

  // Get by sequential search, assuming load factor is < 0.7
  while (chunkOn != NULL) {
    if (chunkOn->chunkX == chunk->chunkX && chunkOn->chunkY == chunk->chunkY && chunkOn->chunkZ == chunk->chunkZ) {
      return;
    }
    cellOn += 1;
    if (cellOn >= map->size) cellOn = 0;
    chunkOn = map->values[cellOn];
  }
  map->values[cellOn] = chunk;
}

void deleteHashMap(struct HashMap* map) {
  free(map->values);
  free(map);
}

unsigned int getHashForIVec3(int x, int y, int z) {
  return (x * 18937) + (y  * 20483) + (z * 29303);
}


