#version  330  core
uniform  sampler2D  diffuseTex;
uniform  sampler2D  diffuseTex2;
uniform  sampler2D  bumpTex;
uniform  sampler2D  bumpTex2;
uniform  vec3    cameraPos;
uniform  vec4    lightColour;
uniform  vec4    lightColour2;
uniform  vec3    lightPos;
uniform  float   lightRadius;
uniform  float   grassLine;
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

	float grassLineCurve = clamp(pow(1.2,IN.worldPos.y - grassLine* abs(normalize(IN.normal).y)),0.0,1.0);
	grassLineCurve = grassLineCurve * abs(normalize(IN.normal).y);

	vec4  diffuse    = texture(diffuseTex , IN.texCoord * 3 ) * (1-grassLineCurve) + texture(diffuseTex2 , IN.texCoord) * vec4(1,1.5,1,1) * (grassLineCurve) ;
	vec3  bumpNormal    = texture(bumpTex , IN.texCoord * 3 ).rgb * (1-grassLineCurve) + texture(bumpTex2 , IN.texCoord ).rgb * (grassLineCurve);

	bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	float  lambert      = max(dot(incident , bumpNormal), 0.0f);
	float  distance     = length(lightPos  - IN.worldPos );
	float  invDistRatio = isPoint == 1 ? lightRadius / distance : 1;
	float  attenuation  =  lightColour.w * invDistRatio * invDistRatio;  // 1.0 - clamp(distance / lightRadius , 0.0,  1.0);

	float  specFactor   = clamp(dot(halfDir , bumpNormal) ,0.0 ,1.0);
	specFactor    = pow(specFactor , 60.0 );

	vec2 fl = vec2((sin(bumpNormal.x * 20)+2)/3,(sin(bumpNormal.z * 20)+1)/2);
	float vl = 0;//(sin((viewDir.x * viewDir.z) * 100)+2)/3;

	float intensity = (IN.worldPos.y > grassLine) ? 1 : (int(bumpNormal.x * 1000) % 100 == 0) ? 2.0 : (int(bumpNormal.x * 1000) % 100 == 1) ? 0.5 : 1;

	vec3  surface = (diffuse.rgb * lightColour.rgb);
	fragColour.rgb = surface * lambert * attenuation * intensity;
	fragColour.rgb += ((lightColour.rgb ) * specFactor )* attenuation *0.01 * intensity;
	fragColour.rgb +=  surface * 0.0f;    // ambient!
	fragColour.a = 0.7;
	//if(IN.worldPos.y > grassLine) fragColour = vec4(1,1,1,1);
}