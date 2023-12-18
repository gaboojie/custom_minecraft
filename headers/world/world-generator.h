#ifndef MINECRAFT_HEADERS_WORLD_WORLD_GENERATOR_H
#define MINECRAFT_HEADERS_WORLD_WORLD_GENERATOR_H

#include "chunk.h"

struct Chunk* createChunk(int chunkX, int chunkY, int chunkZ);
struct Model* generateModelFor(struct Chunk* chunk);

unsigned char isPossibleBlockTransparent(unsigned char x, unsigned char y, unsigned char z);
unsigned char isFaceConsidered(unsigned char x, unsigned char y, unsigned char z, unsigned char faceDirection);
void setFaceConsidered(unsigned char x, unsigned char y, unsigned char z, unsigned char faceDirection);
void resetChunkGeneration();
void addVertex(unsigned int pos[3], unsigned char faceDirection, unsigned char blockType);
void addFace(unsigned char faceDirection, unsigned int v1[3], unsigned int v2[3], unsigned int v3[3], unsigned int v4[3], unsigned char blockType);
unsigned char isSameType(unsigned char blockData, unsigned char x, unsigned char y, unsigned char z);
unsigned int getFaceDirTextureOffset(unsigned char faceDirection);

void processBlock(unsigned char x, unsigned char y, unsigned char z, const char* normal, char faceOffset, unsigned char faceDirection);
unsigned char startXRun(unsigned char x, unsigned char y, unsigned char z, unsigned char blockType, const char* normal, unsigned char faceDirection);
unsigned char startYRun(unsigned char x, unsigned char y, unsigned char z, unsigned char blockType, const char* normal, unsigned char faceDirection);
unsigned char continueWithXYRun(unsigned char x, unsigned char y, unsigned char z, unsigned char x2, unsigned char blockType, const char* normal, unsigned char faceDirection);
unsigned char continueWithYZRun(unsigned char x, unsigned char y, unsigned char z, unsigned char y2, unsigned char blockType, const char* normal, unsigned char faceDirection);
unsigned char continueWithXZRun(unsigned char x, unsigned char y, unsigned char z, unsigned char x2, unsigned char blockType, const char* normal, unsigned char faceDirection);


#endif
