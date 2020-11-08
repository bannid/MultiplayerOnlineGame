#ifndef FONT_PARSER
#define FONT_PARSER
#include "typedefines.h"
#include "win32_fileapi.h"
#include "debug.h"

struct character{
	int32 DecimalCode;
	int32 X;
	int32 Y;
	int32 XOffset;
	int32 YOffset;
	int32 XAdvance;
};

bool parse_font_file(const char* FontFilePath);

#endif