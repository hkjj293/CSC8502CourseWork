#version  330  core
uniform  sampler2D  diffuseTex;
uniform  samplerCube  cubeTex;
uniform  vec3 cameraPos;
uniform float ratio;
uniform float rot;
uniform  mat4 textureMatrix;
uniform vec3 lightPos;

in  Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} IN;

out  vec4  fragColour;

void  main(void)    {
	vec4  diffuse = (texture(diffuseTex , IN.texCoord) + texture(diffuseTex , (-textureMatrix * vec4(IN.texCoord,0,1)).xy) + texture(diffuseTex , (transpose(textureMatrix) * vec4(IN.texCoord,0,1)).xy))/3;
	vec3  viewDir = normalize(cameraPos  - IN.worldPos);

	vec3  reflectDir = reflect(-viewDir ,normalize(IN.normal -  diffuse.rgb * 0.01f));
	float negY = clamp(sin(radians(rot)) + 0.6,0,1);
	float negY2 = clamp(sin(radians(rot*2 + 90)),0,1);
	vec4 tex = texture(cubeTex,(textureMatrix*vec4(reflectDir,1)).xyz) * (1 - clamp(dot(viewDir,vec3(0,1,0))*2,0.0,1.0));
	vec4 col = (tex*tex)*(1-negY)*(1-negY2) + ((negY)*(1-negY2) * (ratio)*(vec4(negY,negY,negY,1.0f) * vec4(0.5,0.6,1.0,1)) + negY2 * (ratio) *(vec4(negY2,negY2,negY2,1.0f) * vec4(0.6,0.4,0.3,1)));
	float ambient = distance(col.xyz,vec3(0,0,0));
	 //vec4  reflectTex = texture(cubeTex,reflectDir);

	 fragColour = (col+ (1 - abs(normalize(viewDir).y)) * vec4(0.05,0.05,0.05,1)) * (1.0f - ratio) * clamp((1 - ((viewDir.y + 1)/2)+ 0.5),0.0,1.0) + (diffuse * ratio) * ambient * (dot(diffuse.rgb,normalize(lightPos))+1) / 2;
	 fragColour.a = 1 - clamp(dot(viewDir,vec3(0,1,0))/2,0.0,1.0);
}