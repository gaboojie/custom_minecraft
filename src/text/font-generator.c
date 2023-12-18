#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "texture.h"
#include "font-generator.h"

float lineHeight = -1;

struct Font* createFontFromFile(char* fontPath, char* texturePath) {
  struct Character* characters = loadCharacterInfo(fontPath);
  struct Texture* texture = createTexture(texturePath);

  return createFont(characters, texture, lineHeight);
}

struct Character* loadCharacterInfo(char* path) {
  struct Character* characters = malloc(128 * sizeof(struct Character));

  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("ERROR COULD NOT READ FONT FILE with path %s", path);
    exit(EXIT_FAILURE);
  }
  int lineOn = 0;
  int charOn = -1;
  while ((read = getline(&line, &len, fp)) != -1) {
    char *token, *str, *tofree;
    tofree = str = strdup(line);

    int tokenOn = 0;
    while ((token = strsep(&str, " "))) {
      if (lineOn == 1 && tokenOn == 1) {
        lineHeight = getNumAfterEquals(token);
      } else if (startsWith(line, "char ")) {
        switch (tokenOn) {
          case 1:
            charOn = getNumAfterEquals(token);
            break;
          case 2:
            characters[charOn].sourceX = getNumAfterEquals(token);
            break;
          case 3:
            characters[charOn].sourceY = getNumAfterEquals(token);
            break;
          case 4:
            characters[charOn].width = getNumAfterEquals(token);
            break;
          case 5:
            characters[charOn].height = getNumAfterEquals(token);
            break;
          case 6:
            characters[charOn].xOffset = getNumAfterEquals(token);
            break;
          case 7:
            characters[charOn].yOffset = getNumAfterEquals(token);
            break;
          case 8:
            characters[charOn].xAdvance = getNumAfterEquals(token);
            break;
          default:
            break;
        }
      }
      tokenOn++;
    }
    free(tofree);
    lineOn++;
  }
  fclose(fp);
  if (line) free(line);

  return characters;
}

float getNumAfterEquals(char* str) {
  int on = 0;
  while (str[on] != '\0') {
      if (str[on] == '=') {
        on++;
        break;
      }
      on++;
  }
  return atof(str + on);
}

char startsWith(char* str, char* toStartWith) {
  int on = 0;
  while (toStartWith[on] != '\0') {
    if (str[on] != toStartWith[on]) return 0;
    on++;
  }
  return 1;
}


