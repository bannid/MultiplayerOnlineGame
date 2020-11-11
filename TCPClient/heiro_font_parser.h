#ifndef FONT_PARSER
#define FONT_PARSER
#include "win32_includes.h"
#include "typedefines.h"
#include "win32_fileapi.h"
#include "debug.h"
#include <stdlib.h>

#define MAX_CHARACTERS 150
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
	// TODO(Banni): Load this from the file
	uint32 TextureWidth = 512;
	uint32 TextureHeight = 512;
	
};
bool parse_font_file(const char* FontFilePath,
					 character_set * Characters);

#endif