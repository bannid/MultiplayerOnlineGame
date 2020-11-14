#version 330 core

in vec2 texCoordOut;

out vec4 fragColor;
uniform sampler2D texture1;
void main(){
	fragColor = texture(texture1,texCoordOut);
	if(fragColor.a < 0.1f){
		discard;
	}
}