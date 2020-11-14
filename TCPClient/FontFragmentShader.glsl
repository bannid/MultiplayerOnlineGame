#version 330 core

in vec2 texCoordOut;

out vec4 fragColor;
uniform sampler2D FontTexture;
uniform vec4 FontColor;
void main(){
	fragColor = texture(FontTexture,texCoordOut);
	fragColor *= FontColor;
	if(fragColor.a < 0.2f){
		discard;
	}
}