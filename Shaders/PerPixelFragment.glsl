#version  330  core
uniform  sampler2D  diffuseTex;
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
	vec3  worldPos;
} IN;

out  vec4  fragColour;

void  main(void)    {
	vec3  incident   = normalize(lightPos  - (isPoint == 1 ? IN.worldPos : vec3(0,0,0)) );
	vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
	vec3  halfDir    = normalize(incident + viewDir );
	vec4  diffuse    = texture(diffuseTex , IN.texCoord );

	 float  lambert      = max(dot(incident , IN.normal), 0.0f);
	 float  distance     = length(lightPos  - IN.worldPos );
	 float  invDistRatio = isPoint == 1 ? lightRadius / distance : 1;
	 float  attenuation  =  lightColour.w * invDistRatio * invDistRatio;  // 1.0 - clamp(distance / lightRadius , 0.0,  1.0);

	 float  specFactor   = clamp(dot(halfDir , IN.normal ) ,0.0 ,1.0);
	 specFactor    = pow(specFactor , 60.0 );

	  vec3  surface = (diffuse.rgb * lightColour.rgb);
	  fragColour.rgb = surface * lambert * attenuation;
	  fragColour.rgb += (lightColour.rgb * specFactor )* attenuation *0.33;
	  fragColour.rgb +=  surface * 0.1f;    // ambient!
	  fragColour.a = diffuse.a;
}