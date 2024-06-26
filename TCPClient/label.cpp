#include "label.h"


void init_label(label * Label,
				gui * ContainerGui,
				char * String,
				color_t Color,
				float Size,
				constraint_value_type SizeType){
	Assert(SizeType != NO_VALUE);
	Assert(ContainerGui != NULL);
    Assert(string_length(String) < MAX_LABEL_SIZE);
    copy_string((const char*)String,
                Label->String);
	Label->Size = Size;
	Label->SizeType = SizeType;
	Label->ContainerGui = ContainerGui;
	Label->Color = Color;
	Label->Alignment = CENTER_HORIZONTAL;
}

void set_size_label(label * Label,
					float Size,
					constraint_value_type SizeType){
	Assert(SizeType != NO_VALUE);
	Label->Size = Size;
	Label->SizeType = SizeType;
}
