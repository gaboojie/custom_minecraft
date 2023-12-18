#include <stdio.h>
#include "world.h"
#include "world-renderer.h"
#include "world-generator.h"
#include "chunk.h"
#include "text-renderer.h"
#include "OpenGL/gl3.h"
#include "hash-map.h"

struct HashMap* chunks;

void createWorld() {
  createTextRenderer();
  createWorldRenderer();
  chunks = createHashMap(100, 0.5f);
}
void updateWorld(double deltaTime) {
  updateWorldRenderer(deltaTime);
}
void renderWorld() {
  glEnable(GL_DEPTH_TEST);
  renderWorldRenderer(chunks);
  glDisable(GL_DEPTH_TEST);
  renderTextRenderer();
}
void deleteWorld() {
  deleteWorldRenderer();
  for (int i = 0; i < chunks->size; i++) {
    if (chunks->values[i] != NULL) deleteChunk(chunks->values[i]);
  }
  deleteHashMap(chunks);
  deleteTextRenderer();
}
