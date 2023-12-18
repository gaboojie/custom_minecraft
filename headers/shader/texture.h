#ifndef MINECRAFT_HEADERS_SHADER_TEXTURE_H
#define MINECRAFT_HEADERS_SHADER_TEXTURE_H

struct Texture {
  unsigned int id;
  int width, height;
};

struct Texture* createTexture(char* path);
struct Texture* createTextureArray(char* path);
void bindTexture(struct Texture* texture);
void bindTextureArray(struct Texture* texture);
void deleteTexture(struct Texture* texture);

#endif
