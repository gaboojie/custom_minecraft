#ifndef MINECRAFT_HEADERS_SHADER_SHADER_H
#define MINECRAFT_HEADERS_SHADER_SHADER_H

#include "OpenGL/gl3.h"
#include "transform.h"

struct ShaderProgram {
  unsigned int id;
  int transformUniform, projectionUniform, viewUniform, colorUniform;
  int blockColorsUniform, blockTexIDsUniform;
};

struct ShaderProgram* createShaderProgram(char* vertexShaderPath, char* fragmentShaderPath, void (*f)(struct ShaderProgram*));
unsigned int createShader(char* path, GLenum SHADER_TYPE, unsigned int program_id);
void bindProgram(struct ShaderProgram* program);
void unbindProgram();
void deleteProgram(struct ShaderProgram* program);
void setUniformLocations(struct ShaderProgram* program);
void setTextUniformLocations(struct ShaderProgram* program);
void loadTransformMatrix(struct ShaderProgram* program, mat4* matrix);
void loadProjectionMatrix(struct ShaderProgram* program, mat4* matrix);
void loadViewMatrix(struct ShaderProgram* program, mat4* matrix);
void loadColor(struct ShaderProgram* program, vec4 value);
void loadBlockTexIDs(struct ShaderProgram* program, unsigned int* value, int numTexCoords);

#endif
