#ifndef MINECRAFT_HEADERS_MATH_MATH_UTIL_H
#define MINECRAFT_HEADERS_MATH_MATH_UTIL_H

#include "cglm/mat4.h"
#include "cglm/vec3.h"
#include "transform.h"

static vec3 X_VECTOR = (vec3) { 1.0f, 0.0f, 0.0f };
static vec3 Y_VECTOR = (vec3) { 0.0f, 1.0f, 0.0f };
static vec3 Z_VECTOR = (vec3) { 0.0f, 0.0f, 1.0f };

mat4* createTransformMatrix(struct Transform* transform, mat4* matrix);
mat4* createViewMatrix(struct Transform* camera, mat4* matrix);
mat4* createProjectionMatrix(float FOV, float aspect, float nearVal, float farVal, mat4* matrix);
void deleteMatrix(mat4* matrix);

#endif
