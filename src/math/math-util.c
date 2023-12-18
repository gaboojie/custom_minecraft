#include "transform.h"
#include "math-util.h"
#include "cglm/affine.h"
#include "cglm/cam.h"

mat4* createTransformMatrix(struct Transform* transform, mat4* matrix) {
  if (matrix == NULL) matrix = (mat4*) malloc(sizeof(float)*16);
  glm_translate_make((vec4 *) matrix, (float *) transform->position);
  vec3* rotation = transform->rotation;
  glm_rotate_x((vec4 *) matrix, (*rotation)[0], (vec4 *) matrix);
  glm_rotate_y((vec4 *) matrix, (*rotation)[1], (vec4 *) matrix);
  glm_rotate_z((vec4 *) matrix, (*rotation)[2], (vec4 *) matrix);
  glm_scale((vec4 *) matrix, (float *) transform->scale);
  return matrix;
}
mat4* createViewMatrix(struct Transform* camera, mat4* matrix) {
  if (matrix == NULL) matrix = (mat4*) malloc(sizeof(float)*16);
  glm_mat4_identity((vec4 *) matrix);
  vec3* rotation = camera->rotation;
  glm_rotate((vec4 *) matrix, (*rotation)[0], X_VECTOR);
  glm_rotate((vec4 *) matrix, (*rotation)[1], Y_VECTOR);
  glm_rotate((vec4 *) matrix, (*rotation)[2], Z_VECTOR);
  vec3* position = camera->position;
  vec3 invPosition = { -1 * (*position)[0], -1 * (*position)[1], -1 * (*position)[2]};
  glm_translate((vec4 *) matrix, (float *) &invPosition);
  return matrix;
}
mat4* createProjectionMatrix(float FOV, float aspect, float nearVal, float farVal, mat4* matrix) {
  if (matrix == NULL) matrix = (mat4*) malloc(sizeof(float)*16);
  glm_mat4_identity((vec4 *) matrix);
  glm_perspective(FOV, aspect, nearVal, farVal, (vec4 *) matrix);
  return matrix;
}

void deleteMatrix(mat4* matrix) {
  free(matrix);
}
