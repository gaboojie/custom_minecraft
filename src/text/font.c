#include <stdlib.h>
#include <stdio.h>
#include "font.h"

float lastTextWidth = -1;
float lastTextHeight = -1;

struct Font* createFont(struct Character* characters, struct Texture* texture, float lineHeight) {
  struct Font* font = (struct Font*) malloc(sizeof (struct Font));
  font->characters = characters;
  font->texture = texture;
  font->lineHeight = lineHeight;
  for (int i = 0; i < 128; ++i) {
    struct Character* character = characters + i;
    calculateFractionalCharacterSize(character, texture->width, texture->height);
    calculateCharacterTexCoords(character);
  }
  return font;
}

void deleteFont(struct Font* font) {
  free(font->characters);
  deleteTexture(font->texture);
  free(font);
}

void calculateFractionalCharacterSize(struct Character* character, float textureWidth, float textureHeight) {
  character->sourceX /= textureWidth;
  character->sourceY /= textureHeight;
  character->width /= textureWidth;
  character->height /= textureHeight;
  character->xOffset /= textureWidth;
  character->yOffset /= textureHeight;
  character->xAdvance /= textureWidth;
}

void calculateCharacterTexCoords(struct Character* character) {
  float farX = character->sourceX + character->width;
  float farY = character->sourceY + character->height;
  character->texCoords[0][0] = character->sourceX;
  character->texCoords[0][1] = farY;
  character->texCoords[1][0] = farX;
  character->texCoords[1][1] = farY;
  character->texCoords[2][0] = farX;
  character->texCoords[2][1] = character->sourceY;
  character->texCoords[3][0] = character->sourceX;
  character->texCoords[3][1] = character->sourceY;
}

//

struct VAO* createTextVAO(char* str, unsigned int str_len, struct Font* font) {
  float* vertices = malloc(str_len * sizeof(float) * 4 * 2);
  int* indices = malloc(str_len * sizeof(int) * 6);
  float* texCoords = malloc(str_len * sizeof(float) * 4 * 2);

  // Calculate the text's width and heights
  lastTextWidth = 0;
  lastTextHeight = 0;
  for (int i = 0; i < str_len; ++i) {
    char on = str[i];
    struct Character character = (font->characters)[on];
    lastTextWidth += character.width;
    lastTextHeight = (lastTextHeight > character.height) ? lastTextHeight : character.height;
  }

  float xOn = -lastTextWidth / 2;
  float yOn = -lastTextHeight / 2;
  for (int i = 0; i < str_len; ++i) {
      char c = str[i];
      struct Character character = (font->characters)[c];
      if (c == 32) {
        xOn += character.xAdvance - 0.01f;
        continue;
      }
      float w = character.width;
      float h = character.height;

      // Vertices
      vertices[i*8] = xOn;
      vertices[i*8+1] = yOn;
      vertices[i*8+2] = xOn + w;
      vertices[i*8+3] = yOn;
      vertices[i*8+4] = xOn + w;
      vertices[i*8+5] = yOn + h;
      vertices[i*8+6] = xOn;
      vertices[i*8+7] = yOn + h;

      // Tex Coords
      texCoords[i*8] =   character.texCoords[0][0]; texCoords[i*8+1] = 1 - character.texCoords[0][1];
      texCoords[i*8+2] = character.texCoords[1][0]; texCoords[i*8+3] = 1 - character.texCoords[1][1];
      texCoords[i*8+4] = character.texCoords[2][0]; texCoords[i*8+5] = 1 - character.texCoords[2][1];
      texCoords[i*8+6] = character.texCoords[3][0]; texCoords[i*8+7] = 1 - character.texCoords[3][1];

      // Indices
      indices[i*6]   = i*4;
      indices[i*6+1] = i*4 + 1;
      indices[i*6+2] = i*4 + 2;
      indices[i*6+3] = i*4;
      indices[i*6+4] = i*4 + 2;
      indices[i*6+5] = i*4 + 3;

      xOn += character.xAdvance - 0.01f;
  }

  return create2DVaoWithIndices(vertices, 8 * str_len, indices, 6 * str_len, texCoords, 8 * str_len);
}