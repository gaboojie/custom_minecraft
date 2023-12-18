#ifndef MINECRAFT_HEADERS_APP_H
#define MINECRAFT_HEADERS_APP_H

#include "GLFW/glfw3.h"
#include "vao.h"

#define INITIAL_WIDTH 1024
#define INITIAL_HEIGHT 768

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void createApp();
void updateApp();
void renderApp();
void finishApp();
void calculateTimings();

#endif
