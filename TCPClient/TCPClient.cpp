/*
TODO List:
-- Write a property parser for guis so we dont have to compile everytime we change something
    for guis.
-- Do the font rendering using the Signed distance fields for better quality.
-- Implement the ability to add various properties onto the guis.
-- Add a titled gui.
 -- Implement the functionality to swap guis dynamically for better memory
    utilization.
 -- Create buttons.
 -- Create input boxes and dynamic array to fit varying size data in it.
 -- Kind of chat_gui_manager that gives us chat windows to chat to other players?
 -- Animations for the guis.
  -- Implement timer class for the guis.
  -- // NOTE(Banni): More things put here....
  
 TODO Nice to have:
 1. Drag and drop feature for the guis.
 2. Fancy animatons?
 3. Assets to c code converter to pack all the resources in single exe.
 4. Sound.
*/
#include "win32_includes.h"
#include "opengl_includes.h"
#include "shadernew.h"
#include "texturenew.h"
#include "vaonew.h"
#include "tcp_client.h"
#include "win32_fileapi.h"
#include "heiro_font_parser.h"
#include "colors.h"
#include "common_client.h"
#include "prop_processor.h"
#include "font_renderer.h"
#include "gui.h"
#include "gui_renderer.h"
#include "label.h"
#include "label_renderer.h"

//Asset paths
#define VERTEX_SHADER_PATH        "..\\TCPClient\\VertexShader.glsl"
#define FRAGMENT_SHADER_PATH      "..\\TCPClient\\SpriteFragmentShader.glsl"
#define FONT_FRAGMENT_SHADER_PATH "..\\TCPClient\\FontFragmentShader.glsl"
#define GUI_FRAGMENT_SHADER_PATH  "..\\TCPClient\\GuiFragmentShader.glsl"
#define FONT_TEXTURE_PATH         "..\\res\\calibri.png"
#define PROP_FILE_PATH            "..\\TCPClient\\guis_layout.prop"
#define FONT_FILE_PATH            "..\\res\\calibri.fnt"

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	return true;
}

void render_guis(gui * MasterGui,
				 draw_context * DrawContext){
	for(int i = 0; i<MasterGui->NumberOfChildren;i++){
		draw_gui(MasterGui->Children[i],
				 DrawContext);
	}
}

void gl_clear_screen(color_t Color){
	glClearColor(Color.r, Color.g, Color.b, Color.a);
	glClear(GL_COLOR_BUFFER_BIT);
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
	
	if(!parse_font_file(FONT_FILE_PATH,
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
	
	int32 SizeOfMemory = MAX_NUMBER_GUIS * sizeof(gui);
	gui * GuiMemory = (gui*)VirtualAlloc(NULL,
										 MAX_NUMBER_GUIS * sizeof(gui),
										 MEM_COMMIT,
										 PAGE_READWRITE);
	initialize_gui_manager(&GlobalGuiManager,
						   GuiMemory);
	GlobalMasterGui = get_memory_for_gui(&GlobalGuiManager);
	//MasterGui does not have any parent.
	init_gui(GlobalMasterGui,
			 "globalMasterGui",
			 GlobalScreenHeight,
			 GlobalScreenWidth,
			 NULL);
	
	gui * PlayerListGui  = get_memory_for_gui(&GlobalGuiManager);
	gui * PlayerListGuiTitle = get_memory_for_gui(&GlobalGuiManager);
	label Label;
	init_gui(PlayerListGui,
			 "playerListGui",
			 0,
			 0,
			 GlobalMasterGui);
	init_gui(PlayerListGuiTitle,
			 "playerListTitleGui",
			 0,
			 0,
			 PlayerListGui);
	init_label(&Label,
			   PlayerListGuiTitle,
			   "Player list",
			   YELLOW,
			   0.1f,
			   RELATIVE_VALUE
			   );
	
	add_constraint_gui(PlayerListGui,{HEIGHT,0.5f,RELATIVE_VALUE});
	add_constraint_gui(PlayerListGui,{WIDTH,0.3f,RELATIVE_VALUE});
	add_constraint_gui(PlayerListGui,{MARGIN_TOP,10,FIXED_VALUE});
	add_constraint_gui(PlayerListGui,{MARGIN_RIGHT,10,FIXED_VALUE});
	
	add_constraint_gui(PlayerListGuiTitle,{HEIGHT,0.1f,RELATIVE_VALUE});
	add_constraint_gui(PlayerListGuiTitle,{WIDTH,0.9f,RELATIVE_VALUE});
	add_constraint_gui(PlayerListGuiTitle,{MARGIN_TOP,0.5f,RELATIVE_VALUE});
	add_constraint_gui(PlayerListGuiTitle,{MARGIN_RIGHT,0.05f,RELATIVE_VALUE});
	
	
	set_background_color_gui(PlayerListGui,
							 WHITE,
							 0.1f);
	set_background_color_gui(PlayerListGuiTitle,
							 RED,
							 0.5f);
	apply_constraints_from_prop_file(PROP_FILE_PATH,
									 GlobalMasterGui);
	while (!glfwWindowShouldClose(Window))
	{
		FontDrawer.ScreenHeight = GlobalScreenHeight;
		FontDrawer.ScreenWidth = GlobalScreenWidth;
		GuiDrawer.ScreenWidth  = GlobalScreenWidth;
		GuiDrawer.ScreenHeight = GlobalScreenHeight;
		
		gl_clear_screen(LAVENDER_BLUSH);
		render_guis(GlobalMasterGui,
					&GuiDrawer);
		
		float FontSize = 22.0f;
		draw_label(&Label,
				   &FontDrawer);
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	glfwTerminate();
#endif
	return 0;
}