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
	if(Label->Alignment == CENTER_HORIZONTAL){
		TopLeftX += TotalWidth / 2;
	}
	draw_string(Label->String,
				TopLeftX,
				TopLeftY,
				DrawContext,
				Label->Color,
				Size);
}