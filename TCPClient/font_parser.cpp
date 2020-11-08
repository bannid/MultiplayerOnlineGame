#include "font_parser.h"
#include "win32_fileapi.h"
#include "typedefines.h"
#include "debug.h"

bool parse_font_file(const char* FontFilePath){
	win32_file FontInfo;
	if(!read_entire_file(FontFilePath,
						 &FontInfo)){
		return false;
	}
	
	char * Ptr = FontInfo.Data;
	char * EndOfFile = FontInfo.Data + FontInfo.FileSize;
	int32 LineLength = 0;
	int32 LinesSkipped = 0;
	//Skip the first 4 lines
	while(LinesSkipped < 4){
		while(*Ptr != '\n'){
			Ptr++;
			Assert(Ptr <= EndOfFile);
		}
		Ptr++;
		Assert(Ptr <= EndOfFile);
		LinesSkipped++;
	}
	char * LineSeeker = Ptr;
	//Find out the length of the current line
	while(*LineSeeker != '\n'){
		LineSeeker++;
		LineLength++;
		Assert(LineSeeker <= EndOfFile);
	}
	
	char * Line = new char[LineLength + 1];
	CopyMemory((PVOID)Line,(VOID*)Ptr,LineLength);
	Line[LineLength] = '\0';
	
	close_file(&FontInfo);
	return true;
}