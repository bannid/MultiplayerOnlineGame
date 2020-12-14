#ifndef BUTTON_H
#define BUTTON_H

#include "gui.h"
#include "string_utils.h"

#define MAX_BUTTON_TEXT_SIZE 20

typedef void (*button_callback)(void*);

struct button{
    gui * ButtonContainerGui;
    char ButtonText[MAX_BUTTON_TEXT_SIZE];
    button_callback Callback;
};

void init_button(button * Button,
                 gui * ButtonConatinerGui,
                 button_callback Callback,
                 const char * ButtonText);

#endif //BUTTON_H
