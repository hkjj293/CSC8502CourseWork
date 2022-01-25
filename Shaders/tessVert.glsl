#version 400 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;
in vec3 normal;

out Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} OUT;

void main(void) {
	OUT.texCoord = texCoord;//(textureMatrix * vec4(texCoord , 0.0,  1.0)). xy;
	OUT.normal = normal;
	OUT.worldPos = position;
	gl_Position = vec4(position , 1.0);
	//OUT.texCoord = texCoord;
}