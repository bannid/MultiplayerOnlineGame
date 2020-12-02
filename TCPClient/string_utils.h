#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "typedefines.h"

bool compare_strings(const char * StringFirst,
                     const char * StringSecond);
int32 string_length(const char * String);
void to_uppercase(char * String);
bool string_contains(const char * String,
                     char * Pattern);

#endif
