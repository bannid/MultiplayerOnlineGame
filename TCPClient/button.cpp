#include "button.h"

void init_button(button * Button,
                 gui * ButtonContainerGui,
                 button_callback Callback,
                 const char * ButtonText){
    Assert(ButtonContainerGui != NULL);
    Assert(string_length(ButtonText)<MAX_BUTTON_TEXT_SIZE);
    Button->ButtonContainerGui = ButtonContainerGui;
    Button->Callback = Callback;
    copy_string(ButtonText,Button->ButtonText);
}