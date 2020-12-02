#include "gui_memory_manager.h"


void initialize_gui_manager(gui_manager * GuiManager,
							gui * GuisMemory){
	Assert(GuisMemory != NULL);
	GuiManager->NumberOfGuis = 0;
	GuiManager->GuisMemory = GuisMemory;
}
gui * get_memory_for_gui(gui_manager * GuiManager){
	Assert(GuiManager->NumberOfGuis < MAX_NUMBER_GUIS);
    gui * PointerToReturn =  GuiManager->GuisMemory + GuiManager->NumberOfGuis++;
    return PointerToReturn;
}
void free_gui_memory(gui*){
	// TODO(Banni): 
}