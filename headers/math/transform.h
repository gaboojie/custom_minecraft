#ifndef MINECRAFT_HEADERS_MATH_TRANSFORM_H
#define MINECRAFT_HEADERS_MATH_TRANSFORM_H

#include "cglm/vec3.h"

struct Transform {
    vec3* position;
    vec3* rotation;
    vec3* scale;
};

struct Transform* createTransform(vec3 position, vec3 rotation, vec3 scale);
void deleteTransform(struct Transform* transform);

#endif
