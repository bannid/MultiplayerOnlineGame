#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
#include "common_client.h"
#include "renderer.h"
#include "colors.h"

void draw_string(const char* String,
				 int32 X,
				 int32 Y,
				 draw_context * DrawContext,
				 color_t Color = WHITE,
				 float Size = 1.0f);
#endif