#include <stdio.h>
#include <stdlib.h>
#include "file-loader.h"
#include "shader.h"
#include "OpenGL/gl3.h"
#include "cglm/mat4.h"

struct ShaderProgram* createShaderProgram(char* vertexShaderPath, char* fragmentShaderPath, void (*f)(struct ShaderProgram*)) {
  // Create shader program
  struct ShaderProgram* program = malloc(sizeof(struct ShaderProgram));
  unsigned int program_id = glCreateProgram();
  program->id = program_id;

  // Add vertex and fragment shaders
  unsigned int vertex_shader = createShader(vertexShaderPath, GL_VERTEX_SHADER, program_id);
  unsigned int fragment_shader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER, program_id);

  // Link program and ensure no errors occurred
  glLinkProgram(program_id);
  int success;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if(!success) {
    char infoLog[512];
    glGetProgramInfoLog(program_id, 512, NULL, infoLog);
    printf("ERROR Linking Shader!");
    fputs(infoLog, stderr);
    exit(1);
  }

  // Discard shader objects, as we do not need them anymore
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Set attribute locations
  (*f)(program);

  return program;
}

unsigned int createShader(char* path, GLenum SHADER_TYPE, unsigned int program_id) {
  unsigned int shader_id = glCreateShader(SHADER_TYPE);
  char* file_contents = load_file(path);
  glShaderSource(shader_id, 1, &file_contents, NULL);
  glCompileShader(shader_id);
  int success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
    printf("ERROR: Compiling shader %s", path);
    fputs(infoLog, stderr);
    free(file_contents);
    exit(1);
  }
  glAttachShader(program_id, shader_id);
  free(file_contents);
  return shader_id;
}

void bindProgram(struct ShaderProgram* program) {
  glUseProgram(program->id);
}
void unbindProgram() {
  glUseProgram(0);
}

void deleteProgram(struct ShaderProgram* program) {
  glDeleteProgram(program->id);
  free(program);
}

void setUniformLocations(struct ShaderProgram* program) {
  program->transformUniform = glGetUniformLocation(program->id, "transform");
  program->viewUniform = glGetUniformLocation(program->id, "view");
  program->projectionUniform = glGetUniformLocation(program->id, "projection");
  program->blockTexIDsUniform = glGetUniformLocation(program->id, "blockTexIDs");
}
void setTextUniformLocations(struct ShaderProgram* program) {
  program->transformUniform = glGetUniformLocation(program->id, "transform");
  program->projectionUniform = glGetUniformLocation(program->id, "projection");
  program->colorUniform = glGetUniformLocation(program->id, "color");
}

void loadTransformMatrix(struct ShaderProgram* program, mat4* matrix) {
  glUniformMatrix4fv(program->transformUniform, 1, GL_FALSE, (float *) matrix);
}
void loadProjectionMatrix(struct ShaderProgram* program, mat4* matrix) {
  glUniformMatrix4fv(program->projectionUniform, 1, GL_FALSE, (float *) matrix);
}
void loadViewMatrix(struct ShaderProgram* program, mat4* matrix) {
  glUniformMatrix4fv(program->viewUniform, 1, GL_FALSE, (float *) matrix);
}
void loadColor(struct ShaderProgram* program, vec4 value) {
  glUniform4fv(program->colorUniform, 1, value);
}
void loadBlockTexIDs(struct ShaderProgram* program, unsigned int* value, int numTexCoords) {
  glUniform1uiv(program->blockTexIDsUniform, numTexCoords, value);
}









