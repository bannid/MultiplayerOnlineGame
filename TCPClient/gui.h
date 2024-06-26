#ifndef GUI_H
#define GUI_H
#include "typedefines.h"
#include "debug.h"
#include "colors.h"
#include "win32_includes.h"
#include "opengl_includes.h"

#define MAX_DIRECT_CHILDREN_PER_GUI 50
#define MAX_CONSTRAINTS 50

enum constraint_type: uint16{
	//Alignment
	CENTER,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	//Margins
	MARGIN_TOP,
	MARGIN_LEFT,
	MARGIN_BOTTOM,
	MARGIN_RIGHT,
	MARGIN,
	//Size
	HEIGHT,
	WIDTH
};

enum constraint_value_type: uint16{
	RELATIVE_VALUE,
	FIXED_VALUE,
	NO_VALUE
};

struct constraint{
	constraint_type Type;
	float Value;
	constraint_value_type ValueType;
};

struct gui{
	const char * Selector;
	
	int32 Height;
	int32 Width;
	int32 TopLeftX;
	int32 TopLeftY;
	
	constraint Constraints[MAX_CONSTRAINTS];
	int32 NumberOfConstraints;
	gui * Parent;
	gui * Children[MAX_DIRECT_CHILDREN_PER_GUI];
	int32 NumberOfChildren;
	
	color_t BackgroundColor;
	float Alpha;
};

void init_gui(gui * Gui,
			  const char * GuiID,
			  uint32 Height,
			  uint32 Width,
			  gui * Parent);
void add_constraint_gui(gui * Gui,
						constraint Constraint);
void add_child_gui(gui * GuiParent,
				   gui * GuiChild);
void set_background_color_gui(gui * Gui,
							  color_t Color,
							  float Alpha = 1.0f);

#endif