#ifndef MINECRAFT_HEADERS_IO_INPUT_H
#define MINECRAFT_HEADERS_IO_INPUT_H

#include "GLFW/glfw3.h"
#include "camera.h"

static float MOUSE_SENSITIVITY = 0.001f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
