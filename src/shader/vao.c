#include <stdlib.h>
#include "vao.h"
#include "OpenGL/gl3.h"

struct VAO *createDataVAO(unsigned int* data, int numVertices) {
  struct VAO *vao = createVAOInstance();
  bindVAO(vao);

  unsigned int dataVBO = createIntegerVBO(data, numVertices, 0, 1);
  vao->numVBOs = 1;
  vao->vboIDsArray = malloc(sizeof(unsigned int)*(vao->numVBOs));
  vao->vboIDsArray[0] = dataVBO;

  vao->numVertices = numVertices;

  unbindVAO();
  return vao;
}

struct VAO *createVAO(float* vertices, int numVertices, unsigned int* data) {
  struct VAO *vao = createVAOInstance();
  bindVAO(vao);

  unsigned int vertexVBO = createVBO(vertices, numVertices, 0, 3);
  unsigned int dataVBO = createIntegerVBO(data, numVertices, 1, 1);
  vao->numVBOs = 2;
  vao->vboIDsArray = malloc(sizeof(unsigned int)*(vao->numVBOs));
  vao->vboIDsArray[0] = vertexVBO;
  vao->vboIDsArray[1] = dataVBO;

  vao->numVertices = numVertices;

  unbindVAO();
  return vao;
}

struct VAO *create2DVaoWithIndices(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, int numTexCoords) {
  struct VAO *vao = createVAOInstance();
  bindVAO(vao);

  unsigned int vertexVBO = createVBO(vertices, numVertices, 0, 2);
  unsigned int eboID = createEBO(indices, numIndices);
  unsigned int texCoordsVBO = createVBO(texCoords, numTexCoords, 1, 2);
  vao->vboIDsArray = malloc(sizeof(float)*3);
  vao->vboIDsArray[0] = vertexVBO;
  vao->vboIDsArray[1] = eboID;
  vao->vboIDsArray[2] = texCoordsVBO;
  vao->numVBOs = 3;
  vao->numVertices = numVertices;

  unbindVAO();
  return vao;
}

struct VAO *createVAOwithIndices(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, int numTexCoords) {
  struct VAO *vao = createVAOInstance();
  bindVAO(vao);

  unsigned int vertexVBO = createVBO(vertices, numVertices, 0, 3);
  unsigned int eboID = createEBO(indices, numIndices);
  unsigned int texCoordsVBO = createVBO(texCoords, numTexCoords, 1, 2);
  vao->vboIDsArray = malloc(sizeof(float)*3);
  vao->vboIDsArray[0] = vertexVBO;
  vao->vboIDsArray[1] = eboID;
  vao->vboIDsArray[2] = texCoordsVBO;
  vao->numVBOs = 3;
  vao->numVertices = numVertices;

  unbindVAO();
  return vao;
}

unsigned int createEBO(unsigned int* data, unsigned int numIndices) {
  unsigned int eboID = -1;
  glGenBuffers(1, &eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), data, GL_STATIC_DRAW);
  return eboID;
}

unsigned int createIntegerVBO(unsigned int* data, unsigned int size, int index_position, int vertex_attrib_size) {
  unsigned int vboID = -1;
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(int)*vertex_attrib_size, data, GL_STATIC_DRAW);
  glVertexAttribIPointer(index_position, vertex_attrib_size, GL_UNSIGNED_INT, vertex_attrib_size * sizeof(int), (void*) 0);
  glEnableVertexAttribArray(index_position);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vboID;
}

unsigned int createVBO(float* data, unsigned int size, int index_position, int vertex_attrib_size) {
  unsigned int vboID = -1;
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(float)*vertex_attrib_size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(index_position, vertex_attrib_size, GL_FLOAT, GL_FALSE, vertex_attrib_size * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(index_position);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vboID;
}

struct VAO *createVAOInstance() {
  struct VAO *vao = malloc(sizeof(struct VAO));

  glGenVertexArrays(1, &(vao->id));
  vao->numVBOs = 0;

  return vao;
}

void bindVAO(struct VAO *vao) {
  glBindVertexArray(vao->id);
}

void unbindVAO() {
  glBindVertexArray(0);
}

void deleteVAO(struct VAO *vao) {
  glDeleteVertexArrays(1, &(vao->id));
  if (vao->numVBOs > 0) glDeleteBuffers(vao->numVBOs, vao->vboIDsArray);
  free(vao->vboIDsArray);
  free(vao);
}
