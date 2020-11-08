#ifndef TEXTURE_H
#define TEXTURE_H
#include "typedefines.h"
#include <glad.h>

struct texture{
	int Width;
	int Height;
	int BytesPerPixel;
	int Channels;
	uint32 Id;
};

bool load_texture(texture * Texture,
				  const char* FileName,
				  uint32 DesiredChannels);

void use_texture(uint32 Id);



#endif