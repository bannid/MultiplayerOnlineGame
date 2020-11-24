#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
#include "common_client.h"
#include "renderer.h"
#include "colors.h"
#include "typedefines.h"

void draw_string(const char* String,
				 int32 X,
				 int32 Y,
				 draw_context * DrawContext,
				 color_t Color = WHITE,
				 float Size = 1.0f);
int32 get_drawn_string_width(const char * String,
							 draw_context * DrawContext,
							 float Size);
#endif