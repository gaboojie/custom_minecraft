#include <math.h>
#include "transform.h"
#include "camera.h"
#include "math-util.h"
#include "GLFW/glfw3.h"

struct Camera* createCamera(struct Transform* transform) {
  struct Camera* camera = malloc(sizeof(struct Camera));
  camera->transform = transform;
  camera->viewMatrix = createViewMatrix(transform, NULL);
  return camera;
}
void deleteCamera(struct Camera* camera) {
  deleteTransform(camera->transform);
  deleteMatrix(camera->viewMatrix);
  free(camera);
}
void move(struct Camera* camera, GLFWwindow* window, double dT) {
  bool updateMatrix = 0;
  vec3* position = camera->transform->position;
  vec3* rotation = camera->transform->rotation;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    moveInFacingDirection(position, rotation, -CAMERA_SPEED * dT);
    updateMatrix = 1;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    moveInFacingDirection(position, rotation, CAMERA_SPEED * dT);
    updateMatrix = 1;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    moveInNonFacingDirection(position, rotation, CAMERA_SPEED * dT);
    updateMatrix = 1;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    moveInNonFacingDirection(position, rotation, -CAMERA_SPEED * dT);
    updateMatrix = 1;
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    (*position)[1] += CAMERA_SPEED * dT;
    updateMatrix = 1;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    (*position)[1] -= CAMERA_SPEED * dT;
    updateMatrix = 1;
  }

  if (updateMatrix) {
    createViewMatrix(camera->transform, camera->viewMatrix);
  }
}

void moveInFacingDirection(vec3* position, vec3* rotation, float amount) {
  (*position)[0] += -amount * sinf((*rotation)[1]);
  (*position)[2] += amount * cosf((*rotation)[1]);
}

void moveInNonFacingDirection(vec3* position, vec3* rotation, float amount) {
  (*position)[0] += -amount * cosf((*rotation)[1]);
  (*position)[2] += -amount * sinf((*rotation)[1]);
}