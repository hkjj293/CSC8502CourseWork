#version 330 core

 uniform  mat4  modelMatrix;
 uniform  mat4  viewMatrix;
 uniform  mat4  projMatrix;
 uniform  mat4  rotation;
 in vec3 position;

out Vertex {
	vec3 viewDir;
} OUT;

void main(void)	{
	vec4 worldPos = modelMatrix * vec4(position.x , position.y ,position.z ,1);
	vec3 pos = worldPos.xyz;
	mat4  invproj = inverse(projMatrix);
	pos.xy *= vec2(invproj [0][0] , invproj [1][1]);  // Stretch out the from NDC to world Coord (if not inverse, it will do the reverse)
	pos.z = -1.0f;
	OUT.viewDir =  (vec4(transpose(mat3(viewMatrix)) * normalize(pos),1)).xyz;
	gl_Position	= worldPos;
}