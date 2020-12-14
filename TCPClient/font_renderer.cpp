#include "font_renderer.h"

inline void draw_char(char C,
                      float TopLeftX,
                      float TopLeftY,
					  draw_context * DrawContext,
					  float Size){
	int32 DecimalCode = (int32)C;
	character Character = DrawContext->CharacterSet.Characters[DecimalCode];
	float CharacterWidth = (float)Character.Width * Size;
	float CharacterHeight = (float)Character.Height * Size;
	float YOffset = (float)Character.YOffset * Size;
    float BottomRightX = TopLeftX + CharacterWidth;
	float BottomRightY = TopLeftY + CharacterHeight;
	
    BottomRightY -= (float)Size *  DrawContext->CharacterSet.Size;
    TopLeftY -= (float)Size *  DrawContext->CharacterSet.Size;
    
	float TexTopLeftX = (float)Character.X / DrawContext->CharacterSet.TextureWidth;
	float TexTopLeftY = (float)Character.Y / DrawContext->CharacterSet.TextureHeight;
	float TexBotRightX =(float) (Character.X + Character.Width) / DrawContext->CharacterSet.TextureWidth;
	float TexBotRightY =(float) (Character.Y + Character.Height)/ DrawContext->CharacterSet.TextureHeight;
	
	float vertices[] = {
		BottomRightX,  TopLeftY,           0.0f,TexBotRightX,TexTopLeftY,  // top right
		BottomRightX,  BottomRightY,       0.0f,TexBotRightX,TexBotRightY,  // bottom right
        TopLeftX,      BottomRightY,       0.0f,TexTopLeftX,TexBotRightY,  // bottom left
        TopLeftX,      TopLeftY,           0.0f,TexTopLeftX,TexTopLeftY   // top left 
    };
	
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
	update_vao(&DrawContext->VertexArrayObject,
			   vertices,
			   sizeof(vertices),
			   indices,
			   sizeof(indices),
			   5 * sizeof(float));
	set_float(&DrawContext->Shader,
			  "ScreenWidth",
			  (float)DrawContext->ScreenWidth);
	set_float(&DrawContext->Shader,
			  "ScreenHeight",
			  (float)DrawContext->ScreenHeight);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_string(const char* String,
				 int32 X,
				 int32 Y,
				 draw_context * DrawContext,
				 color_t Color,
				 float Size){
    float XFloat = (float)X;
    float YFloat = (float)Y;
	use_shader(DrawContext->Shader.ProgramId);
	use_vao(DrawContext->VertexArrayObject.VAO);
	use_texture(DrawContext->Texture.Id);
	set_vec4(&DrawContext->Shader,
			 "FontColor",
			 Color);
	Size = (float)Size / DrawContext->CharacterSet.Size;
	while(*String != '\0'){
		char C = *String;
		int32 DecimalCode = int32(C);
		character Character = DrawContext->CharacterSet.Characters[DecimalCode];
        float XAdvance = (Character.XAdvance * Size);
        float YOffset = (Character.YOffset * Size);
		float XOffset = (Character.XOffset * Size);
		draw_char(*String,
                  XFloat + XOffset,
                  YFloat + YOffset,
				  DrawContext,
				  Size);
		String++;
		XFloat += XAdvance;
	}
}


int32 get_drawn_string_width(const char * String,
							 draw_context * DrawContext,
							 float Size){
	int32 Width = 0;
	Size = (float)Size / DrawContext->CharacterSet.Size;
	while(*String != '\0'){
		char C = *String;
		int32 DecimalCode = int32(C);
		character Character = DrawContext->CharacterSet.Characters[DecimalCode];
		int32 XAdvance = (Character.XAdvance * Size) + 0.5f;
		int32 YOffset = (Character.YOffset * Size) + 0.5f;
		int32 XOffset = (Character.XOffset * Size) + 0.5f;
		String++;
		Width += XAdvance;
	}
	return Width;
}