#ifndef RENDERER_H
#define RENDERER_H
#include "common_client.h"

void draw_rectangle(draw_context * DrawContext,
					int32 TopLeftX,
					int32 TopLeftY,
					int32 BottomRightX,
					int32 BottomRightY);
#endif