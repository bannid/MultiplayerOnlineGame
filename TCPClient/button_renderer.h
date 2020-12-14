#ifndef BUTTON_RENDERER_H
#define BUTTON_RENDERER_H

#include "button.h"
#include "font_renderer.h"
#include "common_client.h"
#include "label.h"
#include "label_renderer.h"
#include "gui.h"

void render_button(button * Button,
                   draw_context * DrawContext);

#endif //BUTTON_RENDERER_H
