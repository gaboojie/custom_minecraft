#ifndef MINECRAFT_HEADERS_STRUCTURE_HASH_MAP_H
#define MINECRAFT_HEADERS_STRUCTURE_HASH_MAP_H

#include "cglm/vec3.h"
#include "chunk.h"

struct HashMap {
  void** values;
  int size;
};

struct HashMap* createHashMap(int size, float loadFactor);
void deleteHashMap(struct HashMap* map);
inline unsigned int getHashForIVec3(int x, int y, int z);

void putChunkInMap(struct Chunk* chunk, struct HashMap* map);
struct Chunk* getChunkAtKeyInMap(int x, int y, int z, struct HashMap* map);

#endif
