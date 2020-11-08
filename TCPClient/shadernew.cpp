#include "shadernew.h"

bool compile_shader(shader * Shader,
					const char* VertexShaderFileName,
					const char* FragmentShaderFileName){
	win32_file VertexShader;
	win32_file FragmentShader;
	if(!read_entire_file(VertexShaderFileName,&VertexShader)){
		return false;
	}
	if(!read_entire_file(FragmentShaderFileName,&FragmentShader)){
		return false;
	}
	for(;;){
		const GLchar * VertexShaderSourceCode = VertexShader.Data;
		const GLchar * FragmentShaderSourceCode = FragmentShader.Data;
		Shader->IdVertexShader = glCreateShader(GL_VERTEX_SHADER);
		Shader->IdFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(Shader->IdVertexShader,
					   1,
					   &VertexShaderSourceCode,
					   NULL);
		glShaderSource(Shader->IdFragmentShader,
					   1,
					   &FragmentShaderSourceCode,
					   NULL);
		glCompileShader(Shader->IdVertexShader);
		glCompileShader(Shader->IdFragmentShader);
		int FragmentShaderSuccess;
		int VertexShaderSuccess;
		int ShaderLinkStatus;
		glGetShaderiv(Shader->IdFragmentShader,
					  GL_COMPILE_STATUS,
					  &FragmentShaderSuccess);
		glGetShaderiv(Shader->IdVertexShader,
					  GL_COMPILE_STATUS,
					  &VertexShaderSuccess);
		if(!VertexShaderSuccess){
			glGetShaderInfoLog(Shader->IdVertexShader,
							   ERROR_BUFFER_SIZE,
							   NULL,
							   Shader->ErrorVertexShader);
			break;
		}
		if(!FragmentShaderSuccess){
			glGetShaderInfoLog(Shader->IdFragmentShader,
							   ERROR_BUFFER_SIZE,
							   NULL,
							   Shader->ErrorFragmentShader);
			break;
		}
		Shader->ProgramId = glCreateProgram();
		glAttachShader(Shader->ProgramId,
					   Shader->IdVertexShader);
		glAttachShader(Shader->ProgramId,
					   Shader->IdFragmentShader);
		glLinkProgram(Shader->ProgramId);
		glGetProgramiv(Shader->ProgramId,
					   GL_LINK_STATUS,
					   &ShaderLinkStatus);
		if(!ShaderLinkStatus){
			break;
		}
		close_file(&VertexShader);
		close_file(&FragmentShader);
		return true;
	}
	close_file(&VertexShader);
	close_file(&FragmentShader);
	glDeleteShader(Shader->IdFragmentShader);
	glDeleteShader(Shader->IdVertexShader);
	return false;
}

void use_shader(uint32 Id){
	glUseProgram(Id);
}

void print_shader_error(shader * Shader){
	OutputDebugStringA(Shader->ErrorFragmentShader);
	OutputDebugStringA(Shader->ErrorVertexShader);
}

void set_int(shader * Shader,
			 const char* UniformName,
			 int Value){
	glUniform1i(glGetUniformLocation(Shader->ProgramId,
									 UniformName), Value);
}

void set_float(shader * Shader,
			   const char* UniformName,
			   float Value){
	glUniform1f(glGetUniformLocation(Shader->ProgramId,
									 UniformName),
				Value);
}

void set_vec4(shader * Shader,
			  const char* UniformName,
			  glm::vec4 Value){
	glUniform4fv(glGetUniformLocation(Shader->ProgramId,
									  UniformName),
				 1,
				 glm::value_ptr(Value));
}
