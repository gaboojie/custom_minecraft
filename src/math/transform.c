#include <stdio.h>
#include "transform.h"


struct Transform* createTransform(vec3 position, vec3 rotation, vec3 scale) {
  struct Transform* transform = malloc(sizeof (struct Transform));
  vec3* posPtr = malloc(sizeof (vec3));
  (*posPtr)[0] = position[0];
  (*posPtr)[1] = position[1];
  (*posPtr)[2] = position[2];
  transform->position = posPtr;
  vec3* rotPtr = malloc(sizeof(vec3));
  (*rotPtr)[0] = rotation[0];
  (*rotPtr)[1] = rotation[1];
  (*rotPtr)[2] = rotation[2];
  transform->rotation = rotPtr;
  vec3* scalePtr = malloc(sizeof(vec3));
  (*scalePtr)[0] = scale[0];
  (*scalePtr)[1] = scale[1];
  (*scalePtr)[2] = scale[2];
  transform->scale = scalePtr;
  return transform;
}

void deleteTransform(struct Transform* transform) {
  if (transform->position) free(transform->position);
  if (transform->rotation) free(transform->rotation);
  if (transform->scale) free(transform->scale);
  free(transform);
}