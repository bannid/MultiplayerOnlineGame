#ifndef LABEL_RENDERER_H
#define LABEL_RENDERER_H

#include "label.h"
#include "font_renderer.h"
#include "colors.h"
#include "common_client.h"
#include "typedefines.h"
#include "string_utils.h"

void draw_label(label * Label,
				font_draw_context * DrawContext);

#endif