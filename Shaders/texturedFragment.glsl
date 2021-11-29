#version 330 core

uniform  sampler2D  diffuseTex;
uniform int isFinal;

in Vertex {
	vec2  texCoord;
} IN;

out vec4 fragColour;

void main(void)	{
		fragColour = texture(diffuseTex , IN.texCoord);
		fragColour = vec4(fragColour.r,fragColour.r,fragColour.r,1);
}