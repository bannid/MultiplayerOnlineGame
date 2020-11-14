#include "renderer.h"

void draw_rectangle(draw_context * DrawContext,
					int32 TopLeftX,
					int32 TopLeftY,
					int32 BottomRightX,
					int32 BottomRightY){
	float vertices[] = {
		BottomRightX,  TopLeftY,           0.0f,1.0f,0.0f,  // top right
		BottomRightX,  BottomRightY,       0.0f,1.0f,1.0f,  // bottom right
        TopLeftX,      BottomRightY,       0.0f,0.0f,1.0f,  // bottom left
        TopLeftX,      TopLeftY,           0.0f,0.0f,0.0f   // top left 
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
	use_shader(DrawContext->Shader.ProgramId);
	use_vao(DrawContext->VertexArrayObject.VAO);
	use_texture(DrawContext->Texture.Id);
	set_float(&DrawContext->Shader,
			  "ScreenWidth",
			  (float)DrawContext->ScreenWidth);
	set_float(&DrawContext->Shader,
			  "ScreenHeight",
			  (float)DrawContext->ScreenHeight);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}