#include "gui_renderer.h"

//In this routine, we will process the gui's 
//constraints and calculate the resulting
//properites of gui such as height and position.
void process_gui_constraints(gui * Gui){
	gui * Pg = Gui->Parent;
	Assert(Pg != NULL);
	
	//A gui would inherit some properties
	//from parent by default.
	Gui->TopLeftX =  Pg->TopLeftX;
	Gui->TopLeftY = Pg->TopLeftY;
	Gui->Height = Pg->Height;
	Gui->Width = Pg->Width;
	
	//First we check if we have some height and 
	//width constraints as most of the other constraints
	//depend on these two.
	for(int i = 0; i<Gui->NumberOfConstraints;i++){
		if(Gui->Constraints[i].Type == HEIGHT){
			
		}
		else if(Gui->Constraints[i].Type == WIDTH){
			
		}
	}
	
	//Now we run the other constraints and calculate
	//the properties, the inherited ones as well.
	for(int i = 0; i<Gui->NumberOfConstraints;i++){
		switch(Gui->Constraints[i].Type){
			
			case CENTER:{
				
				break;
			}
			
			case RIGHT:{
				
				break;
			}
			
			case LEFT:{
				
				break;
			}
			
			case BOTTOM:{
				
				break;
			}
			
			case MARGIN_TOP:{
				
				break;
			}
			
			case MARGIN_RIGHT:{
				
				break;
			}
			
			case MARGIN_LEFT:{
				
				break;
			}
			
			case MARGIN_BOTTOM:{
				
				break;
			}
			
			case MARGIN:{
				
				break;
			}
			
		}
	}
	
}

void draw_gui(gui * Gui,
			  draw_context * DrawContext){
	process_gui_constraints(Gui);
}
