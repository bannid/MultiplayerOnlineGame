#include "prop_processor.h"

#define MAX_LENGTH_FOR_GUI_SELECTOR 50
#define MAX_LINE_LENGTH 30
#define MAX_PROPERTY_LENGTH 30

inline void eat_all_leading_white_space(char ** Ptr){
	while(**Ptr == '\t' ||
		  **Ptr == '\n' ||
		  **Ptr == '\r' ||
		  **Ptr == ' '){
		
		*Ptr +=1;
		
	}
}

inline void read_id(char ** Ptr,
					char * GuiSelector){
	int32 Size = 0;
	while(**Ptr != '{'){
		GuiSelector[Size++] = **Ptr;
		*Ptr+=1;
		Assert(Size <= MAX_LENGTH_FOR_GUI_SELECTOR);
	}
	*Ptr+=1;
	GuiSelector[Size] = '\0';
}

inline int32 get_number_of_constraints(char * Ptr,
									   char * EndOfFilePtr){
	int32 NumberOfConstraints = 0;
	int32 StepsTaken = 0;
	while(*Ptr != '}'){
		Ptr++;
		if(*Ptr == ';'){
			NumberOfConstraints++;
		}
		Assert(Ptr <= EndOfFilePtr);
	}
	return NumberOfConstraints;
}

inline void get_next_constraint(char ** Ptr,
                                char * Output){
    int32 Length = 0;
    while(**Ptr != ';'){
        *Output++ = **Ptr;
        *Ptr+=1;
        Length++;
        Assert(Length < MAX_LINE_LENGTH);
    }
    *Ptr+=1;
    *Output = '\0';
}

inline void get_type_and_value(char * Constraint,
                               char * OutConstraintType,
                               char * OutConstraintValue){
    while(*Constraint != ':'){
        *OutConstraintType++ = *Constraint++;
    }
    Constraint++;
    *OutConstraintType = '\0';
    while(*Constraint != '\0'){
        *OutConstraintValue++ = *Constraint++;
    }
    *OutConstraintValue = '\0';
}

inline bool string_ends_with(char * String,
                             char Token){
    while(*String != '\0'){
        String++;
    }
    return *--String == Token;
}

void apply_constraints_from_prop_file(const char* FileName,
									  gui_manager * GuiManager){
	win32_file PropFile;
	if(read_entire_file(FileName, &PropFile)){
		char * Ptr = PropFile.Data;
		char * EndOfFilePtr = Ptr + PropFile.FileSize;
		char GuiSelector[MAX_LENGTH_FOR_GUI_SELECTOR + 1];
		for(;;){
            eat_all_leading_white_space(&Ptr);
            if(Ptr >= EndOfFilePtr){
                break;
            }
            read_id(&Ptr,
                    GuiSelector);
            eat_all_leading_white_space(&Ptr);
            int32 NumberOfConstraints = get_number_of_constraints(Ptr,
                                                                  EndOfFilePtr);
            Assert(NumberOfConstraints <= MAX_CONSTRAINTS);
            char NextConstraint[MAX_LINE_LENGTH];
            char ConstraintType[MAX_PROPERTY_LENGTH];
            char ConstraintValue[MAX_PROPERTY_LENGTH];
            for(int i = 0; i<NumberOfConstraints;i++){
                get_next_constraint(&Ptr,
                                    NextConstraint);
                get_type_and_value(NextConstraint,
                                   ConstraintType,
                                   ConstraintValue);
                to_uppercase(ConstraintType);
                char * TempConstraintType = ConstraintType;
                char * TempConstraintValue = ConstraintValue;
                eat_all_leading_white_space(&TempConstraintType);
                eat_all_leading_white_space(&TempConstraintValue);
                eat_all_leading_white_space(&Ptr);
                if(compare_strings(TempConstraintType,"CENTER")){
                    
                }
                else if(compare_strings(TempConstraintType,"HEIGHT")){
                    bool IsRelativeValue = string_ends_with(TempConstraintValue,'%');
                    float Value = strtof(TempConstraintValue,NULL);
                    constraint_value_type ValueType;
                    ValueType = IsRelativeValue ? RELATIVE_VALUE : FIXED_VALUE;
                    for(int j = 0; j<GuiManager->NumberOfGuis;j++){
                        gui * Gui = GuiManager->GuisMemory + j;
                        if(compare_strings(Gui->Selector,(const char*)GuiSelector)){
                            add_constraint_gui(Gui,{HEIGHT,Value,ValueType});
                        }
                    }
                    
                }
                else if(compare_strings(TempConstraintType,"WIDTH")){
                    bool IsRelativeValue = string_ends_with(TempConstraintValue,'%');
                    float Value = strtof(TempConstraintValue,NULL);
                    constraint_value_type ValueType;
                    ValueType = IsRelativeValue ? RELATIVE_VALUE : FIXED_VALUE;
                    for(int j = 0; j<GuiManager->NumberOfGuis;j++){
                        gui * Gui = GuiManager->GuisMemory + j;
                        if(compare_strings(Gui->Selector,(const char*)GuiSelector)){
                            add_constraint_gui(Gui,{WIDTH,Value,ValueType});
                        }
                    }
                    
                }
                else if(compare_strings(TempConstraintType,"MARGIN_TOP")){
                    bool IsRelativeValue = string_ends_with(TempConstraintValue,'%');
                    float Value = strtof(TempConstraintValue,NULL);
                    constraint_value_type ValueType;
                    ValueType = IsRelativeValue ? RELATIVE_VALUE : FIXED_VALUE;
                    for(int j = 0; j<GuiManager->NumberOfGuis;j++){
                        gui * Gui = GuiManager->GuisMemory + j;
                        if(compare_strings(Gui->Selector,(const char*)GuiSelector)){
                            add_constraint_gui(Gui,{MARGIN_TOP,Value,ValueType});
                        }
                    }
                    
                }
                else if(compare_strings(TempConstraintType,"MARGIN_RIGHT")){
                    bool IsRelativeValue = string_ends_with(TempConstraintValue,'%');
                    float Value = strtof(TempConstraintValue,NULL);
                    constraint_value_type ValueType;
                    ValueType = IsRelativeValue ? RELATIVE_VALUE : FIXED_VALUE;
                    for(int j = 0; j<GuiManager->NumberOfGuis;j++){
                        gui * Gui = GuiManager->GuisMemory + j;
                        if(compare_strings(Gui->Selector,(const char*)GuiSelector)){
                            add_constraint_gui(Gui,{MARGIN_RIGHT,Value,ValueType});
                        }
                    }
                    
                }
            }
            //Eat the '}' closing bracket
            Ptr++;
        }
		close_file(&PropFile);
	}
	else{
		Assert(0);//Should never hit here.
	}
}