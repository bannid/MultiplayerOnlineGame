#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

out vec2 texCoordOut;

uniform float ScreenWidth;
uniform float ScreenHeight;

/*This code maps the screen space
  to opengl space*/
vec3 MapToOpenglSpace(vec3 ScreenSpace){
	vec3 Output = pos;
	
	float NormalizedX = ScreenSpace.x / ScreenWidth;
	float NormalizedY = ScreenSpace.y / ScreenHeight;
	
	float OpenglMaxX = 1.0f;
	float OpenglMaxY = 1.0f;
	
	float OpenglMinX = -1.0f;
	float OpenglMinY = -1.0f;
	
	Output.x = OpenglMinX * (1.0f - NormalizedX) + 
		OpenglMaxX * NormalizedX;
	Output.y = OpenglMaxY * (1.0f - NormalizedY) + 
		OpenglMinY * NormalizedY;
	
	Output.z = pos.z;
	return Output;
}

void main(){
	gl_Position = vec4(MapToOpenglSpace(pos),1.0f);
	texCoordOut = texCoord;
}