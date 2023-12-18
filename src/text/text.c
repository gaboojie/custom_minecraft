#include <stdlib.h>
#include "text.h"

struct Text* createText(char* str, struct Model* model, float unadjusted_width, float unadjusted_height, vec4 color) {
  struct Text* text = malloc(sizeof (struct Text));
  text->text = str;
  text->model = model;
  text->unadjusted_width = unadjusted_width;
  text->unadjusted_height = unadjusted_height;
  text->color[0] = color[0];
  text->color[1] = color[1];
  text->color[2] = color[2];
  text->color[3] = color[3];
  return text;
}
void deleteText(struct Text* text) {
  deleteModel(text->model);
  free(text->text);
  free(text);
}
float getTextWidth(struct Text* text) {
  return text->unadjusted_width * ((*(text->model->transform->scale))[0]);
}
float getTextHeight(struct Text* text) {
  return text->unadjusted_height * ((*(text->model->transform->scale))[1]);
}
