#include "label_renderer.h"

void draw_label(label * Label,
				draw_context * DrawContext){
	float Size;
	if(Label->SizeType == FIXED_VALUE){
		Size = Label->Size;
	}
	else {
		Size = Label->Size * Label->ContainerGui->Width;
	}
	int32 TotalWidth = get_drawn_string_width(Label->String,
											  DrawContext,
											  Size);
	int32 TopLeftX = Label->ContainerGui->TopLeftX;
	int32 TopLeftY = Label->ContainerGui->TopLeftY;
    switch(Label->Alignment){
        case CENTER_HORIZONTAL:{
            float Padding = (Label->ContainerGui->Width - TotalWidth) / 2.0f;
            TopLeftX += Padding;
            break;
        }
        case LEFT:{
            //Do nothing.
            break;
        }
    }
	draw_string(Label->String,
				TopLeftX,
				TopLeftY,
				DrawContext,
				Label->Color,
				Size);
}