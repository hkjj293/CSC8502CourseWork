#version  330  core
uniform  sampler2D  diffuseTex;
uniform  sampler2D  bumpTex;
uniform  vec3    cameraPos;
uniform  vec4    lightColour;
uniform  vec4    lightColour2;
uniform  vec3    lightPos;
uniform  float   lightRadius;
uniform  int	 isPoint;

in  Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  tangent;
	vec3  binormal;
	vec3  worldPos;
} IN;

out  vec4  fragColour;

void  main(void)    {
	vec3  incident   = normalize(lightPos  - (isPoint == 1 ? IN.worldPos : vec3(0,0,0)) );
	vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
	vec3  halfDir    = normalize(incident + viewDir );
	mat3  TBN        = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec4  diffuse    = texture(diffuseTex , IN.texCoord );
	vec3  bumpNormal    = texture(bumpTex , IN.texCoord ).rgb;
	bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	float  lambert      = max(dot(incident , bumpNormal), 0.0f);
	float  distance     = length(lightPos  - IN.worldPos );
	float  invDistRatio = isPoint == 1 ? lightRadius / distance : 1;
	float  attenuation  =  lightColour.w * invDistRatio * invDistRatio;  // 1.0 - clamp(distance / lightRadius , 0.0,  1.0);

	float  specFactor   = clamp(dot(halfDir , bumpNormal) ,0.0 ,1.0);
	specFactor    = pow(specFactor , 60.0 );

	vec2 fl = vec2((sin(bumpNormal.x * 20)+2)/3,(sin(bumpNormal.z * 20)+1)/2);
	float vl = 0;//(sin((viewDir.x * viewDir.z) * 100)+2)/3;

	vec3  surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert * attenuation;
	fragColour.rgb += ((lightColour.rgb - vec3(fl,vl)) * specFactor )* attenuation *0.33;
	fragColour.rgb +=  surface * 0.1f;    // ambient!
	fragColour.a = diffuse.a;
}