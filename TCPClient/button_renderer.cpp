#include "button_renderer.h"

void render_button(button * Button,
                   draw_context * DrawContext){
    label Label;
    init_label(&Label,
               Button->ButtonContainerGui,
               Button->ButtonText,
               BLACK,
               20.0f,
               FIXED_VALUE);
    draw_label(&Label,
               DrawContext);
    
}