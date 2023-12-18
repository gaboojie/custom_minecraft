#ifndef MINECRAFT_HEADERS_SHADER_MODEL_H
#define MINECRAFT_HEADERS_SHADER_MODEL_H

#include "transform.h"
#include "vao.h"
#include "cglm/mat4.h"

struct Model {
  struct Transform* transform;
  struct VAO* vao;
  mat4* transformationMatrix;
};

struct Model* createModel(struct Transform* transform, struct VAO* vao);
void deleteModel(struct Model* model);

#endif
