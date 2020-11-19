#ifndef FONT_PARSER
#define FONT_PARSER
#include "win32_includes.h"
#include "typedefines.h"
#include "win32_fileapi.h"
#include "debug.h"
#include <stdlib.h>

#define MAX_CHARACTERS 150
#define MAX_LENGTH_FONT_NAME 20

struct character{
	int32 DecimalCode;
	int32 X;
	int32 Y;
	int32 Width;
	int32 Height;
	int32 XOffset;
	int32 YOffset;
	int32 XAdvance;
};
struct character_set{
	character Characters[MAX_CHARACTERS];
	int32 LineHeight;
	int32 Size;
	char FontName[MAX_LENGTH_FONT_NAME];
	char FileName[MAX_LENGTH_FONT_NAME];
	uint32 TextureWidth;
	uint32 TextureHeight;
};
bool parse_font_file(const char* FontFilePath,
					 character_set * Characters);

#endif