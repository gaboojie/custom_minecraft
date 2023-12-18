#ifndef MINECRAFT_HEADERS_TEXT_FONT_H
#define MINECRAFT_HEADERS_TEXT_FONT_H

#include "cglm/vec2.h"
#include "texture.h"
#include "vao.h"

struct Character {
  float sourceX, sourceY, width, height, xOffset, yOffset, xAdvance;
  vec2 texCoords[4];
};
struct Font {
  struct Character* characters;
  struct Texture* texture;
  float lineHeight;
};

struct Font* createFont(struct Character* characters, struct Texture* texture, float lineHeight);
void deleteFont(struct Font* font);
void calculateFractionalCharacterSize(struct Character* character, float textureWidth, float textureHeight);
void calculateCharacterTexCoords(struct Character* character);
struct VAO* createTextVAO(char* str, unsigned int str_len, struct Font* font);

#endif
