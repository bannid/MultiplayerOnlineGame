#include "button.h"

void init_button(button * Button,
                 gui * ButtonGui,
                 button_callback Callback,
                 const char * ButtonText){
    Assert(ButtonGui != NULL);
    Assert(string_length(ButtonText)<MAX_BUTTON_TEXT_SIZE);
    Button->ButtonGui = ButtonGui;
    Button->Callback = Callback;
    copy_string(ButtonText,Button->ButtonText);
}