#ifndef MINECRAFT_HEADERS_TEXT_TEXT_H
#define MINECRAFT_HEADERS_TEXT_TEXT_H

#include "vao.h"
#include "model.h"

struct Text {
  char* text;
  vec4 color;
  struct Model* model;
  float unadjusted_width, unadjusted_height;
};

struct Text* createText(char* str, struct Model* model, float unadjusted_width, float unadjusted_height, vec4 color);
void deleteText(struct Text* text);
float getTextWidth(struct Text* text);
float getTextHeight(struct Text* text);

#endif
