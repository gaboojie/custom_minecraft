
#include <stdio.h>
#include "app.h"
#include "GLFW/glfw3.h"
#include "OpenGL/gl3.h"
#include "input.h"
#include "world.h"
#include "text-renderer.h"

int WIDTH = INITIAL_WIDTH, HEIGHT = INITIAL_HEIGHT;
GLFWwindow* WINDOW_ID;
double previousTimeCounter, previousTimeFPSUpdated, deltaTime;
int FPS = 0, currentFrameCount = 0;

int main(void) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    WINDOW_ID = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft", NULL, NULL);
    if (!WINDOW_ID)
    {
        printf("Failed to create the GLFW window!");
        glfwTerminate();
        return -1;
    }

    /* Bind events */
    glfwSetFramebufferSizeCallback(WINDOW_ID, framebuffer_size_callback);
    glfwSetKeyCallback(WINDOW_ID, key_callback);
    glfwSetCursorPosCallback(WINDOW_ID, mouse_move_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(WINDOW_ID);
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glfwSetInputMode(WINDOW_ID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glEnable(GL_CULL_FACE);

    createApp();
    framebuffer_size_callback(WINDOW_ID, WIDTH, HEIGHT);
    glfwShowWindow(WINDOW_ID);
    previousTimeCounter = glfwGetTime();
    previousTimeFPSUpdated = previousTimeCounter;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(WINDOW_ID))
    {
        /* Render & update here */
        updateApp();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderApp();

        /* Swap front and back buffers */
        glfwSwapBuffers(WINDOW_ID);

        /* Poll for and process events */
        glfwPollEvents();
    }
    finishApp();
    glfwTerminate();
    return 0;
}

void createApp() {
  createWorld();
}

void updateApp() {
  calculateTimings();
  updateWorld(deltaTime);
}

void calculateTimings() {
  // Update FPS and deltaTime
  double currentTime = glfwGetTime();
  deltaTime = currentTime - previousTimeCounter;
  previousTimeCounter = currentTime;
  currentFrameCount += 1;
  if (currentTime - previousTimeFPSUpdated >= 1.0) {
    FPS = currentFrameCount;
    previousTimeFPSUpdated = currentTime;
    currentFrameCount = 0;
    updateFPSText(FPS);
  }
}

void renderApp() {
  renderWorld();
}

void finishApp() {
  deleteWorld();
}

