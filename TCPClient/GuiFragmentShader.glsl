#version 330 core

uniform vec4 Color;
uniform float Alpha;
out vec4 FragColor;
void main(){
	FragColor = Color;
	FragColor *= Alpha;
}