#include "gui.h"

//We can only have one MasterGUI which doesnt
//have any parent
static bool MasterGUIInitialized = false;

void initialize_constraint(constraint * Constraint,
						   constraint_type Type,
						   constraint_value_type ValueType,
						   int32 Value
						   ){
	//Check if mandatory data is being passed in
	switch(Type){
		case MARGIN_TOP:
		case MARGIN_LEFT:
		case MARGIN_RIGHT:
		case MARGIN_BOTTOM:
		case RELATIVE_SIZE:
		case FIXED_SIZE:{
			Assert(Value != NOT_APPLICABLE);
			break;
		}
	}
	Constraint->Type = Type;
	Constraint->Value = Value;
	Constraint->ValueType = ValueType;
}

void init_gui(gui * Gui,
			  uint32 Height,
			  uint32 Width,
			  gui * Parent){
	Gui->Height = Height;
	Gui->Width = Width;
	if(Parent == NULL){
		Assert(MasterGUIInitialized == false);
		MasterGUIInitialized = true;
	}
	else{
		Gui->Parent = Parent;
	}
	Gui->NumberOfConstraints = 0;
}

void add_constraint_gui(gui * Gui,
						constraint Constraint){
	Gui->Constraints[Gui->NumberOfConstraints++] = Constraint;
	Assert(Gui->NumberOfConstraints <= MAX_CONSTRAINTS);
}
