#include "win32_includes.h"
#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shadernew.h"
#include "texturenew.h"
#include "vaonew.h"
#include "tcp_client.h"
#include "win32_fileapi.h"
#include "heiro_font_parser.h"
#include "colors.h"
#include "common_client.h"
#include "font_renderer.h"
#include "gui.h"
#include "gui_renderer.h"

//Asset paths
#define VERTEX_SHADER_PATH   "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\VertexShader.glsl"
#define FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\SpriteFragmentShader.glsl"
#define FONT_FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\FontFragmentShader.glsl"
#define GUI_FRAGMENT_SHADER_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\TCPClient\\GuiFragmentShader.glsl"
#define FONT_TEXTURE_PATH "C:\\Users\\Winny-Banni\\source\\repos\\MultiplayerOnlineGame\\res\\calibri.png"
#define MAX_NUMBER_GUIS 100
#include "gui_memory_manager.h"

static int GlobalScreenWidth;
static int GlobalScreenHeight;
static character_set GlobalCharacterSet;
static gui_manager GlobalGuiManager;
static gui * GlobalMasterGui;

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
	GlobalMasterGui->Height = Height;
	GlobalMasterGui->Width = Width;
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

void render_guis(gui * MasterGui,
				 draw_context * DrawContext){
	for(int i = 0; i<MasterGui->NumberOfChildren;i++){
		draw_gui(MasterGui->Children[i],
				 DrawContext);
	}
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
	draw_context GuiDrawer;
	
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
	
	
	if(!compile_shader(&GuiDrawer.Shader,
					   VERTEX_SHADER_PATH,
					   GUI_FRAGMENT_SHADER_PATH
					   )){
		OutputDebugStringA("GUI shader compilation failed!");
		return -1;
	}
	
	if(!load_texture(&FontDrawer.Texture,
					 FONT_TEXTURE_PATH,
					 4)){
		OutputDebugStringA("Failed to load font texture!");
		return -1;
	}
	
	initialize_vao(&FontDrawer.VertexArrayObject);
	initialize_vao(&GuiDrawer.VertexArrayObject);
	
	// TODO(Banni): Allocate some memory for GUIs upfront.
	int32 SizeOfMemory = MAX_NUMBER_GUIS * sizeof(gui);
	gui * GuiMemory = (gui*)VirtualAlloc(NULL,
										 MAX_NUMBER_GUIS * sizeof(gui),
										 MEM_COMMIT,
										 PAGE_READWRITE);
	initialize_gui_manager(&GlobalGuiManager,
						   GuiMemory);
	GlobalMasterGui = get_memory_for_gui(&GlobalGuiManager);
	//MasterGui does not have any parent.
	GlobalMasterGui->Parent = NULL;
	GlobalMasterGui->Height = GlobalScreenHeight;
	GlobalMasterGui->Width = GlobalScreenWidth;
	while (!glfwWindowShouldClose(Window))
	{
		FontDrawer.ScreenHeight = GlobalScreenHeight;
		FontDrawer.ScreenWidth = GlobalScreenWidth;
		GuiDrawer.ScreenWidth  = GlobalScreenWidth;
		GuiDrawer.ScreenHeight = GlobalScreenHeight;
		
		render_guis(GlobalMasterGui,
					&GuiDrawer);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		float FontSize = 20.0f;
		draw_string("Hey there how are you doing? This is some text",
					0,0,
					&FontDrawer,
					20.0f);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glfwTerminate();
#endif
	return 0;
}