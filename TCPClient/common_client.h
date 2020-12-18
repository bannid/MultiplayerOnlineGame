#ifndef COMMON_CLIENT_H
#define COMMON_CLIENT_H

#include "shadernew.h"
#include "texturenew.h"
#include "vaonew.h"
#include "heiro_font_parser.h"

struct draw_context{
	vao VertexArrayObject;
	shader Shader;
	texture Texture;
	character_set CharacterSet;
	int32 ScreenHeight;
	int32 ScreenWidth;
};

typedef draw_context font_draw_context;
typedef draw_context gui_draw_context;

#endif