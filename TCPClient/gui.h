#ifndef GUI_H
#define GUI_H
#include "typedefines.h"
#include "debug.h"
#include "colors.h"
#include "win32_includes.h"

#define MAX_CONSTRAINTS 50
#define NOT_APPLICABLE  0xFFFFFFFF

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
	RELATIVE_VALUE,
	FIXED_VALUE
};

struct constraint{
	constraint_type Type;
	int32 Value;
	constraint_value_type ValueType;
};

struct gui{
	int32 Height;
	int32 Width;
	constraint Constraints[MAX_CONSTRAINTS];
	int32 NumberOfConstraints;
	gui * Parent;
};

void init_gui(gui * Gui,
			  uint32 Height,
			  uint32 Width,
			  gui * Parent);

void add_constraint_gui(gui * Gui,
						constraint Constraint);

void initialize_constraint(constraint * Constraint,
						   constraint_type Type,
						   constraint_value_type ValueType,
						   int32 Value);
#endif