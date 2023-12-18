#include "input.h"
#include "app.h"
#include "shader.h"
#include "math-util.h"
#include "text-renderer.h"
#include "camera.h"
#include <stdio.h>

extern int WIDTH, HEIGHT;
extern struct ShaderProgram* worldRendererProgram;
extern struct ShaderProgram* textRendererProgram;
extern mat4* projMatrix;
extern struct Camera* camera;
extern int FPS;

char shouldDrawDebugText = 0;

float lastMouseX = (INITIAL_WIDTH / 2.0f), lastMouseY = (INITIAL_HEIGHT / 2.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
  if (key == GLFW_KEY_GRAVE_ACCENT) {
    if (action == GLFW_PRESS) {
      GLint polygonMode;
      glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
      if (polygonMode == GL_FILL )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    }
  }
  if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
    shouldDrawDebugText = !shouldDrawDebugText;
    updateFPSText(FPS);
  }
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
  float xoffset = xpos - lastMouseX;
  float yoffset = ypos - lastMouseY;
  lastMouseX = xpos;
  lastMouseY = ypos;
  vec3* rotation = camera->transform->rotation;
  (*rotation)[0] += yoffset * MOUSE_SENSITIVITY;
  (*rotation)[1] += xoffset * MOUSE_SENSITIVITY;
  createViewMatrix(camera->transform, camera->viewMatrix);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  bindProgram(worldRendererProgram);
  projMatrix = createProjectionMatrix(1.13446f, (float) WIDTH/HEIGHT, 0.1f, 1000, projMatrix);
  loadProjectionMatrix(worldRendererProgram, projMatrix); // 1.13446 = 65 degrees
  unbindProgram();
  bindProgram(textRendererProgram);
  loadProjectionMatrix(textRendererProgram, projMatrix);
  unbindProgram();
}
