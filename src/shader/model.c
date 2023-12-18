#include "vao.h"
#include "transform.h"
#include "model.h"
#include "math-util.h"

struct Model* createModel(struct Transform* transform, struct VAO* vao) {
  struct Model* model = malloc(sizeof (struct Model));
  model->transform = transform;
  model->vao = vao;
  model->transformationMatrix = createTransformMatrix(transform, NULL);
  return model;
}
void deleteModel(struct Model* model) {
  deleteTransform(model->transform);
  deleteVAO(model->vao);
  deleteMatrix(model->transformationMatrix);
  free(model);
}
