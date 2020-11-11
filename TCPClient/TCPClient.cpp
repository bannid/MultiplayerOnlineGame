#include "win32_includes.h"
#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shadernew.h"
#include "texturenew.h"
#include "vaonew.h"
#include "tcp_client.h"
#include "win32_fileapi.h"
#include "heiro_font_parser.h"

#define VERTEX_SHADER_PATH   "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\VertexShader.glsl"
#define FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\SpriteFragmentShader.glsl"

static int GlobalScreenWidth;
static int GlobalScreenHeight;
static texture GlobalFontTexture;
static character_set GlobalCharacterSet;
static vao FontVao;

bool connect_to_server(client *Client) {
	if (initialize_client(Client)) {
		if (client_connect_to_server(Client,
									 "127.0.0.1",
									 "27015") == ERROR_SUCCESS) {
			std::cout << "connected to server!!";
			run_client(Client);
			return true;
		}
	}
	return false;
}

void framebuffer_resize_callback(GLFWwindow *Window,
								 int Width,
								 int Height) {
	GlobalScreenWidth = Width;
	GlobalScreenHeight = Height;
	glViewport(0, 0,
			   Width,
			   Height);
}

bool init_glfw(GLFWwindow **Window,
			   const char* WindowName) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int Width = 600;
	int Height = 600;
	GlobalScreenHeight = Height;
	GlobalScreenWidth = Width;
	*Window = glfwCreateWindow(Width, 
							   Height,
							   WindowName,
							   NULL,
							   NULL);
	if (*Window == NULL) {
		return false;
	}
	glfwMakeContextCurrent(*Window);
	glfwSetFramebufferSizeCallback(*Window, framebuffer_resize_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}
	glViewport(0, 0, Width, Height);
	return true;
}

struct draw_context{
	vao VertexArrayObject;
	shader Shader;
	texture Texture;
};

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
	if(GlobalScreenWidth == 0){
		GlobalScreenWidth = 1;
	}
	if(GlobalScreenHeight == 0){
		GlobalScreenHeight = 1;
	}
	set_float(&DrawContext->Shader,
			  "ScreenWidth",
			  (float)GlobalScreenWidth);
	set_float(&DrawContext->Shader,
			  "ScreenHeight",
			  (float)GlobalScreenHeight);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
inline void draw_char(char C,
					  int32 TopLeftX,
					  int32 TopLeftY,
					  draw_context * DrawContext,
					  float Size = 1.0f){
	float BottomRightX = TopLeftX + 10.0f;
	float BottomRightY = TopLeftY + 10.0f;
	int32 DecimalCode = (int32)C;
	character Character = GlobalCharacterSet.Characters[DecimalCode];
	float TexTopLeftX = (float)Character.X / GlobalCharacterSet.TextureWidth;
	float TexTopLeftY = (float)Character.Y / GlobalCharacterSet.TextureHeight;
	float TexBotRightX =(float) (Character.X + Character.Width) / GlobalCharacterSet.TextureWidth;
	float TexBotRightY =(float) (Character.Y + Character.Height)/ GlobalCharacterSet.TextureHeight;
	
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
#if 1
	update_vao(&DrawContext->VertexArrayObject,
			   vertices,
			   sizeof(vertices),
			   indices,
			   sizeof(indices),
			   5 * sizeof(float));
	use_shader(DrawContext->Shader.ProgramId);
	use_vao(DrawContext->VertexArrayObject.VAO);
	use_texture(DrawContext->Texture.Id);
	if(GlobalScreenWidth == 0){
		GlobalScreenWidth = 1;
	}
	if(GlobalScreenHeight == 0){
		GlobalScreenHeight = 1;
	}
	set_float(&DrawContext->Shader,
			  "ScreenWidth",
			  (float)GlobalScreenWidth);
	set_float(&DrawContext->Shader,
			  "ScreenHeight",
			  (float)GlobalScreenHeight);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
}
int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode) {
	
#if 1	
	GLFWwindow * Window;
	if(!init_glfw(&Window,
				  "Networking game")){
		OutputDebugStringA("Could not init GLFW!");
		return -1;
	}
	if(!parse_font_file("C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.fnt",
						&GlobalCharacterSet)){
		OutputDebugStringA("Failed to load the character set info!");
		return -1;
	}
	draw_context DrawContext;
	if(!compile_shader(&DrawContext.Shader,
					   VERTEX_SHADER_PATH,
					   FRAGMENT_SHADER_PATH
					   )){
		OutputDebugStringA("Shader compilation failed!");
		return -1;
	}
	if(!load_texture(&DrawContext.Texture,
					 "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.png",
					 4)){
		OutputDebugStringA("Failed to load textures!");
		return -1;
	}
	if(!load_texture(&GlobalFontTexture,
					 "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.png",
					 4)){
		OutputDebugStringA("Failed to load font texture!");
		return -1;
	}
	
	initialize_vao(&DrawContext.VertexArrayObject);
    initialize_vao(&FontVao);
	
	while (!glfwWindowShouldClose(Window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
#if 0
        draw_rectangle(&DrawContext,
					   0,0,
					   600,600);
#endif
		draw_char('a',
				  100,
				  100,
				  &DrawContext);
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
	glfwTerminate();
#endif
    return 0;
}