#include "world-renderer.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "camera.h"
#include "cglm/mat4.h"
#include "math-util.h"
#include "GLFW/glfw3.h"
#include "block.h"
#include "stdio.h"
#include "input.h"
#include "world-generator.h"
#include "linked-list.h"

extern GLFWwindow* WINDOW_ID;
struct ShaderProgram* worldRendererProgram;
struct Texture* blocksTexture;
struct Camera* camera;
mat4* projMatrix = NULL;

int RENDER_DISTANCE = 3;
int NUM_CHUNKS_TO_RENDER = -1;
struct Chunk* chunksToRender;
ivec3* chunksToRenderOffsets;

void createWorldRenderer() {
  worldRendererProgram = createShaderProgram("/Users/gabe/CLionProjects/minecraft/res/StaticShader.vs", "/Users/gabe/CLionProjects/minecraft/res/StaticShader.fs", setUniformLocations);
  blocksTexture = createTextureArray("/Users/gabe/CLionProjects/minecraft/res/atlas.png");
  camera = createCamera(createTransform((vec3) { 0, 0, 0}, (vec3) {0, 0, 0}, (vec3) {0, 0, 0}));
  createBlockTypes();
  loadBlockTypes(worldRendererProgram);
  loadChunksToRenderOffsets();
}

void updateWorldRenderer(double deltaTime) {
  move(camera, WINDOW_ID, deltaTime);
}

void renderWorldRenderer(struct HashMap* chunks) {
  updateChunksToRender(chunks);

  bindProgram(worldRendererProgram);
  loadViewMatrix(worldRendererProgram, camera->viewMatrix);
  glActiveTexture(GL_TEXTURE0);
  bindTextureArray(blocksTexture);

  for (int i = 0; i < NUM_CHUNKS_TO_RENDER; i++) {
    struct Chunk chunkOn = chunksToRender[i];
    bindVAO(chunkOn.model->vao);
    loadTransformMatrix(worldRendererProgram, chunkOn.model->transformationMatrix);
    glDrawArrays(GL_TRIANGLES, 0, chunkOn.model->vao->numVertices);
  }

  unbindProgram();
}

void deleteWorldRenderer() {
  deleteCamera(camera);
  deleteMatrix(projMatrix);
  deleteTexture(blocksTexture);
  deleteBlockTypes();
  deleteProgram(worldRendererProgram);
  free(chunksToRender);
  free(chunksToRenderOffsets);
}

//

void updateChunksToRender(struct HashMap* chunks) {
  vec3* pos = camera->transform->position;
  int playerChunkX = (int) (*pos)[0] / 16;
  int playerChunkY = (int) (*pos)[1] / 16;
  int playerChunkZ = (int) (*pos)[2] / 16;

  for (int i = 0; i < NUM_CHUNKS_TO_RENDER; i++) {
    int chunkX = chunksToRenderOffsets[i][0] + playerChunkX;
    int chunkY = chunksToRenderOffsets[i][1] + playerChunkY;
    int chunkZ = chunksToRenderOffsets[i][2] + playerChunkZ;
    struct Chunk* chunkOn = getChunkAtKeyInMap(chunkX, chunkY, chunkZ, chunks);

    if (chunkOn == NULL) {
      chunkOn = createChunk(chunkX, chunkY, chunkZ);
      chunksToRender[i] = *chunkOn;
      putChunkInMap(chunkOn, chunks);
    } else {
      chunksToRender[i] = *chunkOn;
    }
  }
}

void loadChunksToRenderOffsets() {
 // Calculate max num chunks from render distance
 NUM_CHUNKS_TO_RENDER = 2 * (RENDER_DISTANCE * RENDER_DISTANCE) - 2 * (RENDER_DISTANCE) + 1;
 for (int i = 1; i < RENDER_DISTANCE; i++) {
   NUM_CHUNKS_TO_RENDER += 4*(i*i) - 4*i + 2;
 }

  chunksToRenderOffsets = calloc(NUM_CHUNKS_TO_RENDER, sizeof(ivec3));
  chunksToRender = calloc(NUM_CHUNKS_TO_RENDER, sizeof(struct Chunk));

 // BFS from player chunk to other chunks
 // I need to keep track of seen blocks
 // Instead of keeping track, I must move in the direction away from the player
 ivec3* start = malloc(sizeof (ivec3));
  (*start)[0] = 0;
  (*start)[1] = 0;
  (*start)[2] = 0;

  ivec3 offsetArr[6] = { 0, 1, 0, 0, -1, 0, 1, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 1};

 struct ListNode* stack = createListNode(start);
 chunksToRenderOffsets[0][0] = 0;
 chunksToRenderOffsets[0][1] = 0;
 chunksToRenderOffsets[0][2] = 0;
 for (int i = 1; i < NUM_CHUNKS_TO_RENDER; i++) {
   struct ListNode* frontOfStack = stack;
   ivec3* stackOn = frontOfStack->data;
   int frontX = (*stackOn)[0];
   int frontY = (*stackOn)[0];
   int frontZ = (*stackOn)[0];

   for (int offsetOn = 0; offsetOn < 6; offsetOn++) {
      if (i >= NUM_CHUNKS_TO_RENDER) break;
      int x = frontX + offsetArr[offsetOn][0];
      int y = frontY + offsetArr[offsetOn][1];
      int z = frontZ + offsetArr[offsetOn][2];

      int distance = (abs(x) + abs(y) + abs(z)) - (abs(frontX) + abs(frontY) + abs(frontZ));
      if (distance > 0) {
        ivec3* toAddToStack = malloc(sizeof (ivec3));
        (*toAddToStack)[0] = x;
        (*toAddToStack)[1] = y;
        (*toAddToStack)[2] = z;
        chunksToRenderOffsets[i][0] = x;
        chunksToRenderOffsets[i][1] = y;
        chunksToRenderOffsets[i][2] = z;
        addNodeToList(stack, createListNode(toAddToStack));
        i++;
      }
   }
   stack = stack->next;
   free(frontOfStack->data);
   free(frontOfStack);
 }
 while (stack != NULL) {
   struct ListNode* on = stack;
   stack = stack->next;
   free(on->data);
   free(on);
 }

 for (int i = 0; i < NUM_CHUNKS_TO_RENDER; i++) {
   printf("%d: %d, %d, %d\n", i, chunksToRenderOffsets[i][0], chunksToRenderOffsets[i][1], chunksToRenderOffsets[i][2]);
 }
}
