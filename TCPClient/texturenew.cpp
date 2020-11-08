#include "texturenew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool load_texture(texture * Texture,
				  const char * FileName,
				  uint32 DesiredChannels){
	stbi_set_flip_vertically_on_load(false);
	uchar* Data = stbi_load(FileName,
							&Texture->Width,
							&Texture->Height,
							&Texture->Channels,
							DesiredChannels);
	if(Data == NULL){
		return false;
	}
	uint32 Format = Texture->Channels == 4 ? GL_RGBA : GL_RGB;
	glGenTextures(1, &Texture->Id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture->Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 Format,
				 Texture->Width,
				 Texture->Height,
				 0,
				 Format,
				 GL_UNSIGNED_BYTE,
				 Data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(Data);
	return true;
}

void use_texture(uint32 Id){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,
				  Id);
}