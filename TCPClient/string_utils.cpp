#include "string_utils.h"

int32 string_length(char * String){
    int32 StringLength = 0;
    while(*String != '\0'){
        StringLength++;
        String++;
    }
    return StringLength;
}

bool compare_strings(char * StringFirst,
                     char * StringSecond){
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