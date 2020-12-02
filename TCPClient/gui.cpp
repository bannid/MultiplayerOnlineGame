#include "gui.h"

//We can only have one MasterGUI which doesnt
//have any parent
static bool MasterGUIInitialized = false;

void init_gui(gui * Gui,
			  const char * GuiSelector,
			  uint32 Height,
			  uint32 Width,
			  gui * Parent){
	Gui->Selector = GuiSelector;
	Gui->Height = Height;
	Gui->Width = Width;
	Gui->BackgroundColor = WHITE;
	Gui->Alpha = 1.0f;
	if(Parent == NULL){
		Assert(MasterGUIInitialized == false);
		MasterGUIInitialized = true;
	}
	else{
		Gui->Parent = Parent;
		add_child_gui(Parent,
					  Gui);
	}
	Gui->NumberOfConstraints = 0;
	Gui->NumberOfChildren = 0;
}

void set_background_color_gui(gui * Gui,
							  color_t Color,
							  float Alpha){
	Gui->BackgroundColor = Color;
	Gui->Alpha = Alpha;
}

void add_constraint_gui(gui * Gui,
						constraint Constraint){
	Gui->Constraints[Gui->NumberOfConstraints++] = Constraint;
	Assert(Gui->NumberOfConstraints <= MAX_CONSTRAINTS);
}
void add_child_gui(gui * GuiParent,
				   gui * GuiChild){
	Assert(GuiParent->NumberOfChildren < MAX_DIRECT_CHILDREN_PER_GUI);
	GuiParent->Children[GuiParent->NumberOfChildren++] = GuiChild;
	GuiChild->Parent = GuiParent;
}