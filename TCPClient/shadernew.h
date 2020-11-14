#ifndef SHADER_NEW_H
#define SHADER_NEW_H
#include "win32_includes.h"
#include "typedefines.h"
#include "win32_fileapi.h"
#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ERROR_BUFFER_SIZE 512

struct shader{
	uint32 ProgramId;
	uint32 IdVertexShader;
	uint32 IdFragmentShader;
	char ErrorFragmentShader[ERROR_BUFFER_SIZE];
	char ErrorVertexShader[ERROR_BUFFER_SIZE];
	bool FragmentShaderInError = false;
	bool VertexShaderInError = false;
};

bool compile_shader(shader * Shader,
					const char* VertexShaderFileName,
					const char* FragmentShaderFileName);
void use_shader(uint32 Id);
void print_shader_error(shader * Shader);
void set_int(shader * Shader,
			 const char* UniformName,
			 int Value);
void set_float(shader * Shader,
			   const char* UniformName,
			   float Value);
void set_vec4(shader * Shader,
			  const char* uniformName,
			  glm::vec4 Value);

#endif