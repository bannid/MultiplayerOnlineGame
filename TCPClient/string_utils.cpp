#include "string_utils.h"

int32 string_length(const char * String){
    int32 StringLength = 0;
    while(*String != '\0'){
        StringLength++;
        String++;
    }
    return StringLength;
}

bool compare_strings(const char * StringFirst,
                     const char * StringSecond){
    if(string_length(StringFirst) != string_length(StringSecond)){
        return false;
    }
    while(*StringFirst != '\0'){
        if(*StringFirst != *StringSecond){
            return false;
        }
        StringFirst++;
        StringSecond++;
    }
    return true;
}

void to_uppercase(char * String){
    while(*String != '\0'){
        int32 DecimalCode = (int32)*String;
        if(DecimalCode >= 97 && DecimalCode <= 122){
            *String = (char)(DecimalCode - 32);
        }
        String++;
    }
}


void copy_string(const char * Source,
                 char * Destination){
    while(*Source != '\0'){
        *Destination++ = *Source++;
    }
    *Destination = '\0';
}

void forward_string_pointer_upto_token(char ** Pointer,
                                       char Token){
    while(**Pointer != Token){
        *Pointer += 1;
        Assert(**Pointer != '\n');
    }
    //Eat the token as well
    *Pointer += 1;
}