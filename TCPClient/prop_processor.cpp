#include "prop_processor.h"

#define MAX_LENGTH_FOR_GUI_ID 50
#define MAX_LINE_LENGTH 30
#define MAX_PROPERTY_LENGTH 30

inline void eat_all_white_space(char ** Ptr){
	while(**Ptr == '\t' ||
		  **Ptr == '\n' ||
		  **Ptr == '\r' ||
		  **Ptr == ' '){
		
		*Ptr +=1;
		
	}
}

inline void read_id(char ** Ptr,
					char * GuiId){
	int32 Size = 0;
	while(**Ptr != '{'){
		GuiId[Size++] = **Ptr;
		*Ptr+=1;
		Assert(Size <= MAX_LENGTH_FOR_GUI_ID);
	}
	*Ptr+=1;
	GuiId[Size] = '\0';
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

void get_next_constraint(char * Ptr,
                         char * Output){
    int32 Length = 0;
    while(*Ptr != ';'){
        *Output++ = *Ptr++;
        Length++;
        Assert(Length < MAX_LINE_LENGTH);
    }
    *Output = '\0';
}

void get_type_and_value(char * Constraint,
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

void apply_constraints_from_prop_file(const char* FileName,
									  gui * MasterGui){
	win32_file PropFile;
	if(read_entire_file(FileName, &PropFile)){
		
		char * Ptr = PropFile.Data;
		char * EndOfFilePtr = Ptr + PropFile.FileSize;
		char GuiId[MAX_LENGTH_FOR_GUI_ID + 1];
		eat_all_white_space(&Ptr);
		read_id(&Ptr,
				GuiId);
		eat_all_white_space(&Ptr);
		int32 NumberOfConstraints = get_number_of_constraints(Ptr,
                                                              EndOfFilePtr);
        char NextConstraint[MAX_LINE_LENGTH];
        char ConstraintType[MAX_PROPERTY_LENGTH];
        char ConstraintValue[MAX_PROPERTY_LENGTH];
        get_next_constraint(Ptr,
                            NextConstraint);
        get_type_and_value(NextConstraint,
                           ConstraintType,
                           ConstraintValue);
        Assert(NumberOfConstraints <= MAX_CONSTRAINTS);
		close_file(&PropFile);
	}
	else{
		Assert(0);//Should never hit here.
	}
}