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
#include "colors.h"
#include "common_client.h"
#include "font_renderer.h"

#define VERTEX_SHADER_PATH   "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\VertexShader.glsl"
#define FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\SpriteFragmentShader.glsl"
#define FONT_FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\FontFragmentShader.glsl"
static int GlobalScreenWidth;
static int GlobalScreenHeight;
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
	draw_context FontDrawer;
	if(!parse_font_file("C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.fnt",
						&FontDrawer.CharacterSet)){
		OutputDebugStringA("Failed to load the character set info!");
		return -1;
	}
	
	
	if(!compile_shader(&FontDrawer.Shader,
					   VERTEX_SHADER_PATH,
					   FONT_FRAGMENT_SHADER_PATH
					   )){
		OutputDebugStringA("Font shader compilation failed!");
		return -1;
	}
	if(!load_texture(&FontDrawer.Texture,
					 "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.png",
					 4)){
		OutputDebugStringA("Failed to load font texture!");
		return -1;
	}
	
	initialize_vao(&FontDrawer.VertexArrayObject);
	while (!glfwWindowShouldClose(Window))
	{
		FontDrawer.ScreenHeight = GlobalScreenHeight;
		FontDrawer.ScreenWidth = GlobalScreenWidth;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		draw_string("Hey there how are you doing? This is some text",
					100,100,
					&FontDrawer,
					0.4f);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glfwTerminate();
#endif
	return 0;
}