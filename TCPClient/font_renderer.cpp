#include "font_renderer.h"

inline void draw_char(char C,
					  int32 TopLeftX,
					  int32 TopLeftY,
					  draw_context * DrawContext,
					  float Size){
	int32 DecimalCode = (int32)C;
	character Character = DrawContext->CharacterSet.Characters[DecimalCode];
	float CharacterWidth = (float)Character.Width * Size;
	float CharacterHeight = (float)Character.Height * Size;
	
	float BottomRightX = TopLeftX + CharacterWidth;
	float BottomRightY = TopLeftY + CharacterHeight;
	
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
				 float Size){
	use_shader(DrawContext->Shader.ProgramId);
	use_vao(DrawContext->VertexArrayObject.VAO);
	use_texture(DrawContext->Texture.Id);
	set_vec4(&DrawContext->Shader,
			 "FontColor",
			 WHITE);
	Size = (float)Size / DrawContext->CharacterSet.Size;
	while(*String != '\0'){
		char C = *String;
		int32 DecimalCode = int32(C);
		character Character = DrawContext->CharacterSet.Characters[DecimalCode];
		int32 XAdvance = (Character.XAdvance * Size) + 0.5f;
		int32 YOffset = (Character.YOffset * Size) + 0.5f;
		int32 XOffset = (Character.XOffset * Size) + 0.5f;;
		draw_char(*String,
				  X + XOffset,
				  Y + YOffset,
				  DrawContext,
				  Size);
		String++;
		X += XAdvance;
	}
}
