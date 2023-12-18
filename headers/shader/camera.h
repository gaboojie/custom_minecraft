#ifndef MINECRAFT_HEADERS_SHADER_CAMERA_H
#define MINECRAFT_HEADERS_SHADER_CAMERA_H

#include "transform.h"
#include "cglm/mat4.h"
#include "GLFW/glfw3.h"

static const float CAMERA_SPEED = 4.0f;

struct Camera {
  struct Transform* transform;
  mat4* viewMatrix;
};

struct Camera* createCamera(struct Transform* transform);
void deleteCamera(struct Camera* camera);
void move(struct Camera* camera, GLFWwindow* window, double dT);

void moveInFacingDirection(vec3* position, vec3* rotation, float amount);
void moveInNonFacingDirection(vec3* position, vec3* rotation, float amount);

#endif
