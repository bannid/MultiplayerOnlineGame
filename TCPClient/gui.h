#ifndef GUI_H
#define GUI_H
#include "typedefines.h"
#include "debug.h"
#include "colors.h"

#define MAX_CONSTRAINTS 50
#define NOT_APPLICABLE 0xFFFFFFFF

enum constraint_type: uint16{
	//Alignment
	CENTER,
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
	RELATIVE_SIZE,
	FIXED_SIZE
};

enum constraint_value_type: uint16{
	RELATIVE,
	FIXED
}

struct constraint{
	constraint_type Type;
	int32 Value;
	constraint_value_type ValueType;
};

struct gui{
	int32 Height;
	int32 Width;
	constraint Constraints[MAX_CONSTRAINTS];
	uint32 NumberOfConstraints;
	gui * ChildGuis;
	gui * Parent;
	int32 NumberOfChildren;
};

void init_gui(gui * Gui,
			  uint32 Height,
			  uint32 Width,
			  int32 TopLeftX,
			  int32 TopLeftY,
			  gui * Parent);

bool add_constraint_gui(gui * Gui,
						constraint Constraint);

void initialize_constraint(constraint * Constraint,
						   constraint_type Type,
						   constraint_value_type ValueType,
						   int32 Value);
#endif