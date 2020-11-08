#include "font_parser.h"
#define MAX_WORD_SIZE 30

void get_next_word_from_line(char ** Line,
							 char * Output,
							 char Splitter){
	uint32 Size = 0;
	while(**Line == ' '){
		*Line += 1;
	}
	while(**Line != Splitter){
		*Output = **Line;
		Output++;
		*Line += 1;
		Size++;
	}
	*Output = '\0';
	Size++;
	Assert(Size <= MAX_WORD_SIZE);
}

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
	
	char * Line = NULL;
	uint32 MemoryAllocatedForLine = 0;
	char * LineSeeker = Ptr;
	
	//Find out the length of the current line
	while(*LineSeeker != '\n'){
		LineSeeker++;
		LineLength++;
		Assert(LineSeeker <= EndOfFile);
	}
	if(Line == NULL ||
	   MemoryAllocatedForLine < LineLength){
		if(Line == NULL){
			//Have the line length set to the double of this line.
			Line = new char[LineLength * 2];
			MemoryAllocatedForLine = LineLength * 2;
		}
		else{
			//If we already have assinged the memory 
			//for line but its less then required.
			delete Line;
			Line = new char[LineLength * 2];
			MemoryAllocatedForLine = LineLength * 2;
		}
	}
	
	CopyMemory((PVOID)Line,(VOID*)Ptr,LineLength);
	Line[LineLength] = '\0';
	//We need to free the memory held by line
	char * LinePtr = Line;
	char Word[MAX_WORD_SIZE];
	//We dont care about word 'char'
	get_next_word_from_line(&LinePtr,Word,' ');
	//Here we should get 'id={x}'
	get_next_word_from_line(&LinePtr,Word,' ');
	
	get_next_word_from_line(&LinePtr,Word,' ');
	
	Assert(Line != NULL);
	delete Line;
	close_file(&FontInfo);
	return true;
}