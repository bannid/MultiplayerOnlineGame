
#include "heiro_font_parser.h"
#define MAX_WORD_SIZE 30

void get_next_word_from_line(char ** Line,
							 char * Output){
	uint32 Size = 0;
	while(**Line == ' '){
		*Line += 1;
	}
	while(**Line != ' ' &&
		  **Line != '\r' &&
		  **Line != '\n'){
		*Output = **Line;
		Output++;
		*Line += 1;
		Size++;
	}
	while(**Line == ' ' ||
		  **Line == '\r' ||
		  **Line == '\n'){
		*Line+=1;
	}
	*Output = '\0';
	Size++;
	Assert(Size <= MAX_WORD_SIZE);
}

void parse_attribute(char * Word,
					 char * AttributeType,
					 char * AttributeValue){
	int32 Size = 0;
	while(*Word != '='){
		*AttributeType = *Word;
		Word++;
		AttributeType++;
		Size++;
	}
	*Word++;
	*AttributeType = '\0';
	while(*Word != '\0'){
		*AttributeValue = *Word;
		Word++;
		AttributeValue++;
		Size++;
	}
	*AttributeValue = '\0';
	Assert(Size <= MAX_WORD_SIZE);
}

void copy_filename_face(char * Source,
						char * Destination){
	int32 Size = 0;
	while(*Source != '\0'){
		if(*Source == '"'){
			Source++;
			continue;
		}
		*Destination = *Source;
		Destination++;
		Source++;
		Assert(Size < MAX_WORD_SIZE);
	}
	*Destination = '\0';
}

bool heiro_parse_font_file(const char* FontFilePath,
                           character_set * CharacterSet){
	win32_file FontInfo;
	if(!read_entire_file(FontFilePath,
						 &FontInfo)){
		return false;
	}
	char * MetaDataPtr = FontInfo.Data;
	//Read the meta data from the file
	while(*MetaDataPtr != '\n'){
		char MetaData[MAX_WORD_SIZE];
		get_next_word_from_line(&MetaDataPtr,
								MetaData);
		if(compare_strings("info",
						   MetaData)){
			continue;
		}
		if(compare_strings("common",MetaData)){
			continue;
		}
		if(compare_strings("chars",MetaData)){
			break;
		}
		char AttributeType[MAX_WORD_SIZE];
		char AttributeValue[MAX_WORD_SIZE];
		parse_attribute(MetaData,
						AttributeType,
						AttributeValue);
		if(compare_strings("size",AttributeType)){
			CharacterSet->Size = strtol(AttributeValue,NULL,10);
		}
		if(compare_strings("lineHeight",AttributeType)){
			CharacterSet->LineHeight = strtol(AttributeValue,NULL,10);
		}
		if(compare_strings("face",AttributeType)){
			copy_filename_face(AttributeValue,
							   CharacterSet->FontName);
		}
		if(compare_strings("file",AttributeType)){
			copy_filename_face(AttributeValue,
							   CharacterSet->FileName);
		}
		if(compare_strings("scaleW",AttributeType)){
			CharacterSet->TextureWidth = strtol(AttributeValue,NULL,10);
		}
		if(compare_strings("scaleH",AttributeType)){
			CharacterSet->TextureHeight = strtol(AttributeValue,NULL,10);
		}
	}
	char * Ptr = FontInfo.Data;
	char * EndOfFile = FontInfo.Data + FontInfo.FileSize;
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
	for(uint32 j = 0; j<94; j++){
		char * LineSeeker = Ptr;
		int32 LineLength = 0;
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
				Line = (char*)VirtualAlloc(NULL, 
										   LineLength * 2,
										   MEM_COMMIT,
										   PAGE_READWRITE);
				Assert(Line != NULL);
				MemoryAllocatedForLine = LineLength * 2;
			}
			else{
				//If we already have assinged the memory 
				//for line but its less than required.
				VirtualFree(Line,
							MemoryAllocatedForLine,
							MEM_RELEASE);
				Line = (char*)VirtualAlloc(NULL, 
										   LineLength * 2,
										   MEM_COMMIT,
										   PAGE_READWRITE);
				Assert(Line != NULL);
				MemoryAllocatedForLine = LineLength * 2;
			}
		}
		
		CopyMemory((PVOID)Line,(VOID*)Ptr,LineLength);
		Ptr+= LineLength + 1;
		Line[LineLength] = '\0';
		//We need to free the memory held by line
		char * LinePtr = Line;
		char Word[MAX_WORD_SIZE];
		//We dont care about word 'char'
		get_next_word_from_line(&LinePtr,Word);
		heiro_character * Character = NULL;
		//Ten attributes per character
		for(int32 i = 0; i<10; i++){
			get_next_word_from_line(&LinePtr,Word);
			char AttributeType[MAX_WORD_SIZE];
			char AttributeValue[MAX_WORD_SIZE];
			parse_attribute(Word,
							AttributeType,
							AttributeValue);
			
			if(compare_strings("id",AttributeType)){
				int32 DecimalCode = strtol(AttributeValue,NULL,10);
				Assert(DecimalCode <= MAX_CHARACTERS);
				Character = CharacterSet->Characters + DecimalCode;
				Character->DecimalCode = DecimalCode;
				continue;
			}
			//This section should never be hit with
			//null character pointer.
			Assert(Character != NULL);
			if(compare_strings("x",AttributeType)){
				Character->X = strtol(AttributeValue,NULL,10); 
				continue;
			}
			
			if(compare_strings("y",AttributeType)){
				Character->Y = strtol(AttributeValue,NULL,10);
				continue;
			}
			
			
			if(compare_strings("width",AttributeType)){
				Character->Width = strtol(AttributeValue,NULL,10);
				continue;
			}
			
			
			if(compare_strings("height",AttributeType)){
				Character->Height = strtol(AttributeValue,NULL,10);
				continue;
			}
			
			
			if(compare_strings("xoffset",AttributeType)){
				Character->XOffset = strtol(AttributeValue,NULL,10);
				continue;
			}
			
			if(compare_strings("yoffset",AttributeType)){
				Character->YOffset = strtol(AttributeValue,NULL,10);
				continue;
			}
			
			
			if(compare_strings("xadvance",AttributeType)){
				Character->XAdvance = strtol(AttributeValue,NULL,10);
				continue;
			}
		}
	}
	Assert(Line != NULL);
	VirtualFree(Line,
				MemoryAllocatedForLine,
				MEM_RELEASE);
	close_file(&FontInfo);
	return true;
}