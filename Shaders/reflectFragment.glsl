#version  330  core
uniform  sampler2D  diffuseTex;
uniform  samplerCube  cubeTex;
uniform  vec3    cameraPos;
uniform float ratio;
in  Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} IN;

out  vec4  fragColour;

void  main(void)    {
	 vec4  diffuse = texture(diffuseTex , IN.texCoord);
	 vec3  viewDir = normalize(cameraPos  - IN.worldPos);

	 vec3  reflectDir = reflect(-viewDir ,normalize(IN.normal - diffuse.rgb * 0.02f));
	 vec4  reflectTex = texture(cubeTex,reflectDir);

	 fragColour = reflectTex * (1.0f - ratio) + (diffuse * ratio);
	 fragColour.a = 0.8;
}