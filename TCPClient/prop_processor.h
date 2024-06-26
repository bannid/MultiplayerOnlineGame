#ifndef PROP_PROCESSOR_H
#define PROP_PROCESSOR_H

#include "win32_fileapi.h"
#include "gui.h"
#include "gui_memory_manager.h"
#include "string_utils.h"
#include <stdlib.h>


void apply_constraints_from_prop_file(const char* FileName,
									  gui_manager * GuiManager);

#endif