#ifndef GUI_MEMORY_MANAGER
#define GUI_MEMORY_MANAGER

#include "gui.h"
#include "typedefines.h"

#if !defined(MAX_NUMBER_GUIS)
#define MAX_NUMBER_GUIS 50
#endif

struct gui_manager{
	gui * GuisMemory;
	int32 NumberOfGuis;
	int32 OccupiedSlots[MAX_NUMBER_GUIS];
};

void initialize_gui_manager(gui_manager * GuiManager,
							gui * GuisMemory);
gui * get_memory_for_gui(gui_manager * GuiManager);
void free_gui_memory(gui*);

#endif