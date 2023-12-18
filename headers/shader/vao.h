#ifndef MINECRAFT_HEADERS_SHADER_VAO_H
#define MINECRAFT_HEADERS_SHADER_VAO_H

struct VAO {
  unsigned int id;
  unsigned int* vboIDsArray;
  unsigned int numVertices;
  unsigned char numVBOs;
};

struct VAO *createDataVAO(unsigned int* data, int numVertices);
struct VAO *createVAO(float* vertices, int numVertices, unsigned int* data);
struct VAO *create2DVaoWithIndices(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, int numTexCoords);
struct VAO *createVAOwithIndices(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, int numTexCoords);
unsigned int createIntegerVBO(unsigned int* data, unsigned int size, int index_position, int vertex_attrib_size);
unsigned int createVBO(float* data, unsigned int size, int index_position, int vertex_attrib_size);
unsigned int createEBO(unsigned int* data, unsigned int size);
struct VAO *createVAOInstance();
void bindVAO(struct VAO *vao);
void unbindVAO();
void deleteVAO(struct VAO *vao);

#endif
