#ifndef MINECRAFT_HEADERS_TEXT_FONT_GENERATOR_H
#define MINECRAFT_HEADERS_TEXT_FONT_GENERATOR_H

struct Font* createFontFromFile(char* fontPath, char* texturePath);
struct Character* loadCharacterInfo(char* path);
float getNumAfterEquals(char* str);
char startsWith(char* str, char* toStartWith);

#endif
