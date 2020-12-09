#ifndef LABEL_H
#define LABEL_H
#include "gui.h"
#include "colors.h"
#include "string_utils.h"

#define MAX_LABEL_SIZE 100
struct label{
	char String[MAX_LABEL_SIZE + 1];
	gui * ContainerGui;
	float Size;
	constraint_value_type SizeType;
	color_t Color;
	constraint_type Alignment;
};

void init_label(label * Label,
				gui * ContainerGui,
				char * String,
				color_t Color,
				float Size,
				constraint_value_type SizeType);
void set_size_label(label * Label,
					float Size,
					constraint_value_type SizeType);

#endif