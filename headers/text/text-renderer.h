#ifndef MINECRAFT_HEADERS_TEXT_RENDERER_H
#define MINECRAFT_HEADERS_TEXT_RENDERER_H

#include "model.h"
#include "text.h"

void createTextRenderer();
void renderTextRenderer();
void deleteTextRenderer();
void addText(char* str, unsigned int str_len, struct Transform* model, vec4 color);
void updateFPSText(int FPS);

#endif
