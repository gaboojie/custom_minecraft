#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OpenGL/gl3.h"

struct Texture* createTextureArray(char* path) {
  // Load image data by STB from path
  int texture_width, texture_height, numberOfChannels;
  stbi_set_flip_vertically_on_load(1);
  unsigned char* data = stbi_load(path, &texture_width, &texture_height, &numberOfChannels, 4);
  if (!data) {
    printf("ERROR: Failed to load texture %s", path);
    exit(1);
  }

  // Load image data into OpenGL
  unsigned int texture_id;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);

  const int tilesX = 32;
  const int tilesY = 16;
  const int tileW = 512 / tilesX;
  const int tileH = 256 / tilesY;
  const int tileSizeX = tileW * 4;
  const int rowLen = tilesX * tileSizeX;
  unsigned char tile[tileW * tileH * 4];

  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, tileW, tileH, tilesX * tilesY, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);

  for (int y = 0; y < tilesY; y++) {
    for (int x = 0; x < tilesX; x++) {
      for (int row = 0; row < tileH; row++) {
        for (int i = 0; i < tileSizeX; i++) {
          tile[row*tileSizeX+i] = data[y*tileH*rowLen + row*rowLen + x*tileSizeX+i];
        }
      }
      int i = (tilesY-y-1) * tilesX + x;
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tileW, tileH, 1, GL_RGBA, GL_UNSIGNED_BYTE, tile);
    }
  }
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  stbi_image_free(data);

  // Create texture from OpenGL data
  struct Texture* texture = malloc(sizeof(struct Texture));
  texture->id = texture_id;
  texture->width = texture_width;
  texture->height = texture_height;
  return texture;
}

struct Texture* createTexture(char* path) {
  // Load image data by STB from path
  int texture_width, texture_height, numberOfChannels;
  stbi_set_flip_vertically_on_load(1);
  unsigned char* data = stbi_load(path, &texture_width, &texture_height, &numberOfChannels, 4);
  if (!data) {
    printf("ERROR: Failed to load texture %s", path);
    exit(1);
  }

  // Load image data into OpenGL
  unsigned int texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_image_free(data);

  // Create texture from OpenGL data
  struct Texture* texture = malloc(sizeof(struct Texture));
  texture->id = texture_id;
  texture->width = texture_width;
  texture->height = texture_height;
  return texture;
}

void bindTextureArray(struct Texture* texture) {
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id);
}

void bindTexture(struct Texture* texture) {
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

void deleteTexture(struct Texture* texture) {
  glDeleteTextures(1, &texture->id);
  free(texture);
}