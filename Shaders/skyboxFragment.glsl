#version  330  core

uniform  samplerCube  cubeTex;
uniform  samplerCube  cubeTex2;
uniform  mat4  rotation;
uniform float rot;
in  Vertex {
	vec3  viewDir;
} IN;

out  vec4  fragColour;

void  main(void)    {
	float negY = clamp(sin(radians(rot)) + 0.6,0,1);
	float negY2 = clamp(sin(radians(rot*2 + 90)),0,1);
	vec4 tex = texture(cubeTex ,normalize(rotation * vec4(IN.viewDir,1)).xyz);
	vec4 tex2 = texture(cubeTex2,normalize(IN.viewDir));
	float l = (length(tex2)/length(vec4(1,1,1,1))) > 0.634 ? 1 : 0;

	fragColour = (tex*tex)*(1-negY)*(1-negY2)*l  + 
			3 * (((negY)*(1-negY2)*(vec4(negY,negY,negY,1.0f)* vec4(0.3,0.4,0.5,1) ) +
			negY2 *(vec4(negY2,negY2,negY2,1.0f) * vec4(0.6,0.4,0.3,1))))* tex2 +
	 		(1 - abs(normalize(IN.viewDir).y)) * vec4(0.1,0.1,0.1,1) * vec4(tex2.xyz,1) * vec4(tex2.xyz,1);

	//fragColour *= 5;
}