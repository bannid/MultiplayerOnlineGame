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
			if(Gui->Constraints[i].ValueType == RELATIVE_VALUE){
				Gui->Height = Pg->Height * Gui->Constraints[i].Value;
			}
			else if(Gui->Constraints[i].ValueType == FIXED_VALUE){
				Gui->Height = (int32)Gui->Constraints[i].Value;
			}
		}
		else if(Gui->Constraints[i].Type == WIDTH){
			if(Gui->Constraints[i].ValueType == RELATIVE_VALUE){
				Gui->Width = Pg->Width * Gui->Constraints[i].Value;
			}
			else if(Gui->Constraints[i].ValueType == FIXED_VALUE){
				Gui->Width = (int32)Gui->Constraints[i].Value;
			}
		}
	}
	
	//Now we run the other constraints and calculate
	//the properties, the inherited ones as well.
	for(int i = 0; i<Gui->NumberOfConstraints;i++){
		switch(Gui->Constraints[i].Type){
			
			case CENTER:{
				Gui->TopLeftX += Gui->Parent->Width / 2 - Gui->Width / 2;
				Gui->TopLeftY += Gui->Parent->Height/ 2 - Gui->Height / 2;
				break;
			}
			
			case CENTER_HORIZONTAL:{
				Gui->TopLeftX += Gui->Parent->Width / 2 - Gui->Width / 2;
				break;
			}
			
			case CENTER_VERTICAL:{
				Gui->TopLeftY += Gui->Parent->Height/ 2 - Gui->Height / 2;
				break;
			}
			
			case RIGHT:{
				Gui->TopLeftX = Gui->Parent->Width - Gui->Width;
				break;
			}
			
			case LEFT:{
				Gui->TopLeftX = Gui->Parent->TopLeftX;
				break;
			}
			
			case BOTTOM:{
				Gui->TopLeftY = Gui->Parent->Height - Gui->Height;
				break;
			}
			
			case MARGIN_TOP:{
				constraint_value_type ValueType = Gui->Constraints[i].ValueType;
				Assert(ValueType != NO_VALUE);
				if(ValueType == FIXED_VALUE){
					Gui->TopLeftY += Gui->Constraints[i].Value;
				}
				else{
					Gui->TopLeftY += Gui->Constraints[i].Value * Gui->Height;
				}
				break;
			}
			
			case MARGIN_LEFT:{
				constraint_value_type ValueType = Gui->Constraints[i].ValueType;
				Assert(ValueType != NO_VALUE);
				if(ValueType == FIXED_VALUE){
					Gui->TopLeftX += Gui->Constraints[i].Value;
				}
				else{
					Gui->TopLeftX += Gui->Constraints[i].Value * Gui->Width;
				}
				break;
			}
			case MARGIN_BOTTOM:{
				constraint_value_type ValueType = Gui->Constraints[i].ValueType;
				Assert(ValueType != NO_VALUE);
				float Value = Gui->Constraints[i].Value;
				if(ValueType == FIXED_VALUE){
					Gui->TopLeftY += Gui->Parent->Height - Gui->Height - Value;
					break;
				}
				else{
					Gui->TopLeftY += Gui->Parent->Height - Gui->Height - (Value * Gui->Height);
				}
			}
			
			case MARGIN_RIGHT:{
				constraint_value_type ValueType = Gui->Constraints[i].ValueType;
				Assert(ValueType != NO_VALUE);
				float Value = Gui->Constraints[i].Value;
				if(ValueType == FIXED_VALUE){
					Gui->TopLeftX += Gui->Parent->Width - Gui->Width - Value;
					break;
				}
				else{
					Gui->TopLeftX += Gui->Parent->Width - Gui->Width - (Value * Gui->Width);
				}
				break;
			}
			
			case MARGIN:{
				// TODO(Banni): Maybe we should change the dimensions of this gui
				// to put a margin acorss all the sides?
				// TODO(Banni): Implement
				Assert(0);
				break;
			}
			
		}
	}
	
}
// NOTE(Banni): This function may result in stack overflow
void draw_gui(gui * Gui,
			  draw_context * DrawContext){
	use_shader(DrawContext->Shader.ProgramId);
	process_gui_constraints(Gui);
	set_vec4(&DrawContext->Shader,
			 "Color",
			 Gui->BackgroundColor);
	set_float(&DrawContext->Shader,
			  "Alpha",
			  Gui->Alpha);
	draw_rectangle(DrawContext,
				   Gui->TopLeftX,
				   Gui->TopLeftY,
				   Gui->TopLeftX + Gui->Width,
				   Gui->TopLeftY + Gui->Height);
	for(int i = 0; i<Gui->NumberOfChildren; i++){
		draw_gui(Gui->Children[i],
				 DrawContext);
	}
}
