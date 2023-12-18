#ifndef MINECRAFT_HEADERS_WORLD_WORLD_RENDERER_H
#define MINECRAFT_HEADERS_WORLD_WORLD_RENDERER_H

#include "chunk.h"
#include "hash-map.h"

void createWorldRenderer();
void updateWorldRenderer(double deltaTime);
void renderWorldRenderer(struct HashMap* chunks);
void deleteWorldRenderer();

void updateChunksToRender(struct HashMap* chunks);
void loadChunksToRenderOffsets();

#endif
