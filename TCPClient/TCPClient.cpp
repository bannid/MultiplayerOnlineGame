/*
TODO List:
 -- Create buttons
-- Do the font rendering using the Signed distance fields for better quality.
-- Add the functionality to provide colors by their names in prop file defined in colors.h file.
-- Add a titled gui.
 -- Implement the functionality to swap guis dynamically for better memory
    utilization.
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
#define MAX_NUMBER_GUIS 100

#include <stdio.h>
#include "win32_includes.h"
#include "opengl_includes.h"
#include "wgl.h"
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
#include "gui_memory_manager.h"
#include "gui.h"
#include "gui_renderer.h"
#include "label.h"
#include "label_renderer.h"
#include "button.h"
#include "button_renderer.h"

//Asset paths
#define VERTEX_SHADER_PATH        "..\\TCPClient\\VertexShader.glsl"
#define FRAGMENT_SHADER_PATH      "..\\TCPClient\\SpriteFragmentShader.glsl"
#define FONT_FRAGMENT_SHADER_PATH "..\\TCPClient\\FontFragmentShader.glsl"
#define GUI_FRAGMENT_SHADER_PATH  "..\\TCPClient\\GuiFragmentShader.glsl"
#define FONT_TEXTURE_PATH         "..\\res\\calibri.png"
#define PROP_FILE_PATH            "..\\TCPClient\\guis_layout.prop"
#define FONT_FILE_PATH            "..\\res\\calibri.fnt"

static int GlobalScreenWidth;
static int GlobalScreenHeight;

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

bool WGLExtensionSupported(const char *extension_name) {
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;
    
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    
    if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL) {
        return(false);
    }
    
    return(true);
}

bool enable_vsync(){
    PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
    PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;
    
    if (WGLExtensionSupported("WGL_EXT_swap_control"))
    {
        // Extension is supported, init pointers.
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        
        // this is another function from WGL_EXT_swap_control extension
        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
        //Enable Vsync;
        wglSwapIntervalEXT(1);
        return(true);
    }
    return(false);
}

void TempCallbackFunction(void * Data){
    DEBUG_OUTPUT("Hey there I am callback function");
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode) {
	bool FrameRateLockedToMonitorRefreshRate = false;
    GLFWwindow * Window;
	if(!init_glfw(&Window,
				  "Networking game")){
		OutputDebugStringA("Could not init GLFW!");
		return -1;
	}
	draw_context FontDrawer;
	draw_context GuiDrawer;
	gui_manager GuiMemoryManager;
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
    gui * GuisMemory = (gui*)VirtualAlloc(NULL,
                                          MAX_NUMBER_GUIS * sizeof(gui),
                                          MEM_COMMIT,
                                          PAGE_READWRITE);
    initialize_gui_manager(&GuiMemoryManager,
                           GuisMemory);
	GlobalMasterGui = get_memory_for_gui(&GuiMemoryManager);
	//MasterGui does not have any parent.
	init_gui(GlobalMasterGui,
			 "globalMasterGui",
			 GlobalScreenHeight,
			 GlobalScreenWidth,
			 NULL);
	
	gui * PlayerListGui  = get_memory_for_gui(&GuiMemoryManager);
	gui * PlayerListGuiTitle = get_memory_for_gui(&GuiMemoryManager);
    gui * FrameRateGui = get_memory_for_gui(&GuiMemoryManager);
    gui * PreviousButtonGui = get_memory_for_gui(&GuiMemoryManager);
    gui * NextButtonGui = get_memory_for_gui(&GuiMemoryManager);
	label Label;
    label FrameRateLabel;
    button PreviousButton;
    button NextButton;
    init_button(&PreviousButton,
                PreviousButtonGui,
                TempCallbackFunction,
                "<");
    init_button(&NextButton,
                NextButtonGui,
                TempCallbackFunction,
                ">");
    init_gui(FrameRateGui,
             "frameRateGui",
             0,
             0,
             GlobalMasterGui);
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
    init_gui(PreviousButtonGui,
             "previousButtonGui",
             0,
             0,
             PlayerListGui);
    init_gui(NextButtonGui,
             "nextButtonGui",
             0,
             0,
             PlayerListGui);
	init_label(&Label,
			   PlayerListGuiTitle,
			   "Player list",
			   BLACK,
			   0.1f,
			   RELATIVE_VALUE
               );
    init_label(&FrameRateLabel,
			   FrameRateGui,
               "Last frame time: %fms",
			   BLACK,
			   20.0f,
			   FIXED_VALUE
			   );
	apply_constraints_from_prop_file(PROP_FILE_PATH,
                                     &GuiMemoryManager);
    float Timer1 = 0;
    float Timer2 = 0;
    float ElapsedTimeInMs = 0;
    float ElapsedTimeInSeconds = 0;
    Timer1 = glfwGetTime();
    FrameRateLockedToMonitorRefreshRate = enable_vsync();
	while (!glfwWindowShouldClose(Window))
	{
		FontDrawer.ScreenHeight = GlobalScreenHeight;
		FontDrawer.ScreenWidth = GlobalScreenWidth;
		GuiDrawer.ScreenWidth  = GlobalScreenWidth;
		GuiDrawer.ScreenHeight = GlobalScreenHeight;
		if(glfwGetKey(Window,GLFW_KEY_M) == GLFW_PRESS){
            apply_constraints_from_prop_file(PROP_FILE_PATH,
                                             &GuiMemoryManager);
        }
		gl_clear_screen(LAVENDER_BLUSH);
		render_guis(GlobalMasterGui,
                    &GuiDrawer);
        render_button(&PreviousButton,
                      &FontDrawer);
        render_button(&NextButton,
                      &FontDrawer);
		draw_label(&Label,
                   &FontDrawer);
        draw_label(&FrameRateLabel,
                   &FontDrawer);
		glfwSwapBuffers(Window);
		glfwPollEvents();
#if defined(DEBUG)
        //Timing
        Timer2 = glfwGetTime();
        ElapsedTimeInSeconds = Timer2 - Timer1;
        ElapsedTimeInMs = ElapsedTimeInSeconds * 1000.0f;
        sprintf(FrameRateLabel.String,"Last frame time: %fms",ElapsedTimeInMs);
        Assert(string_length(FrameRateLabel.String)<MAX_LABEL_SIZE);
        Timer1 = Timer2;
#endif
	}
	glfwTerminate();
	return 0;
}