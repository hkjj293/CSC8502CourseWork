#version 400 core
layout(vertices = 4) out; //num vertices in output patch
uniform int level;

in Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} IN[]; //Equal to size of the draw call vertex count

out Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} OUT []; // Equal to the size of the layout vertex count

void main() {
	//float level =  GL_MAX_TESS_GEN_LEVEL;
	gl_TessLevelInner [0] = level;
	gl_TessLevelInner [1] = level;
	gl_TessLevelOuter [0] = level;
	gl_TessLevelOuter [1] = level;
	gl_TessLevelOuter [2] = level;
	gl_TessLevelOuter [3] = level;
	OUT[gl_InvocationID ]. colour = IN[gl_InvocationID ]. colour;
	OUT[gl_InvocationID ]. texCoord = IN[gl_InvocationID ]. texCoord;
	OUT[gl_InvocationID ]. normal = IN[gl_InvocationID ]. normal;
	OUT[gl_InvocationID ]. worldPos = IN[gl_InvocationID ]. worldPos;
	gl_out[gl_InvocationID ]. gl_Position = gl_in[gl_InvocationID ]. gl_Position;
}