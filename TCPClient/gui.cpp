#include "gui.h"

//We can only have one MasterGUI which doesnt
//have any parent
static bool MasterGUIInitialized = false;

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
	Gui->NumberOfChildren = 0;
}

void add_constraint_gui(gui * Gui,
						constraint_type ConstraintType,
						float ConstraintValue,
						constraint_value_type ConstraintValueType){
	constraint Constraint;
	Constraint.Type = ConstraintType;
	Constraint.Value = ConstraintValue;
	Constraint.ValueType = ConstraintValueType;
	Gui->Constraints[Gui->NumberOfConstraints++] = Constraint;
	Assert(Gui->NumberOfConstraints <= MAX_CONSTRAINTS);
}
void add_child(gui * GuiParent,
			   gui * GuiChild){
	Assert(GuiParent->NumberOfChildren < MAX_DIRECT_CHILDREN_PER_GUI);
	GuiParent->Children[GuiParent->NumberOfChildren++] = GuiChild;
	GuiChild->Parent = GuiParent;
}