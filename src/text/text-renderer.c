#include <stdio.h>
#include <string.h>
#include "text-renderer.h"
#include "font-generator.h"
#include "font.h"
#include "linked-list.h"
#include "shader.h"
#include "input.h"

struct ShaderProgram* textRendererProgram;
struct Font* font;
struct ListNode* textList = NULL;
extern mat4* projMatrix;

extern float lastTextWidth;
extern float lastTextHeight;
extern char shouldDrawDebugText;

void createTextRenderer() {
  font = createFontFromFile("/Users/gabe/CLionProjects/minecraft/res/mc.fnt", "/Users/gabe/CLionProjects/minecraft/res/mc.png");
  textRendererProgram = createShaderProgram("/Users/gabe/CLionProjects/minecraft/res/TextShader.vs", "/Users/gabe/CLionProjects/minecraft/res/TextShader.fs", setTextUniformLocations);
  struct Transform* transform = createTransform((vec3) {-0.77f, 0.6f, 0}, (vec3) {0, 0, 0}, (vec3) {0.35f, 0.35f, 1.0f});
  char* text = malloc(sizeof("FPS: 0"));
  strcpy(text, "FPS: 0");
  addText(text, sizeof("FPS: 0"), transform, (vec4) {0, 0, 0, 0});
}
void renderTextRenderer() {
  bindProgram(textRendererProgram);
  glActiveTexture(GL_TEXTURE0);
  bindTexture(font->texture);
  struct ListNode* on = textList;
  while (on != NULL) {
    struct Text* textOn = (on->data);
    bindVAO(textOn->model->vao);
    loadTransformMatrix(textRendererProgram, textOn->model->transformationMatrix);
    loadColor(textRendererProgram, textOn->color);
    glDrawElements(GL_TRIANGLES, textOn->model->vao->numVertices * 3, GL_UNSIGNED_INT, 0);
    on = on->next;
  }
  unbindProgram();
}
void deleteTextRenderer() {
  deleteFont(font);
  deleteProgram(textRendererProgram);
  struct ListNode* on = textList;
  while (on != NULL) {
    struct ListNode* prev = on;
    on = on->next;
    deleteText(prev->data);
    free(prev);
  }
}

void addText(char* str, unsigned int str_len, struct Transform* transform, vec4 color) {
  struct VAO* vao = createTextVAO(str, str_len, font);
  struct Model* model = createModel(transform, vao);
  struct Text* text = createText(str, model, lastTextWidth, lastTextHeight, color);
  struct ListNode* node = createListNode(text);
  textList = addNodeToList(textList, node);
}

void updateFPSText(int FPS) {
  struct Text* fpsText = textList->data;
  free(fpsText->text);
  deleteVAO(fpsText->model->vao);
  char fps_str[6];
  int size = sprintf(fps_str, "%d", FPS);
  char* text = malloc(size + 5);
  fpsText->color[3] = shouldDrawDebugText;
  strcpy(text, "FPS: ");
  strcpy(text + 5, fps_str);
  fpsText->text = text;
  fpsText->model->vao = createTextVAO(text, 5 + size, font);
}
