#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "typedefines.h"
#include "debug.h"

bool compare_strings(const char * StringFirst,
                     const char * StringSecond);
int32 string_length(const char * String);
void to_uppercase(char * String);
bool string_contains(const char * String,
                     char * Pattern);
void substring(const char * String,
               int32 StartIndex,
               int32 EndIndex);
void copy_string(const char * Source,
                 char * Destination);
void forward_string_pointer_upto_token(char ** Pointer,
                                       char Token);
#endif
