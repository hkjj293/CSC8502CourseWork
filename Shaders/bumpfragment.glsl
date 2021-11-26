#version  330  core
uniform  sampler2D  diffuseTex;
uniform  sampler2D  diffuseTex2;
uniform  sampler2D  bumpTex;
uniform  sampler2D  bumpTex2;
uniform  vec3    cameraPos;
uniform  vec4    lightColour [128];
uniform  vec3    lightPos [128];
uniform  float   lightRadius [128];
uniform  vec3	 lightDirection [128];
uniform  int	 lightType [128];
uniform  int	 lightnum;

uniform  float   grassLine;

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
	float grassLineCurve = clamp(pow(1.2,IN.worldPos.y - grassLine* abs(normalize(IN.normal).y)),0.0,1.0);
	grassLineCurve = grassLineCurve * abs(normalize(IN.normal).y);
	vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
	mat3  TBN        = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	vec4  diffuse    = texture(diffuseTex , IN.texCoord * 3 ) * (1-grassLineCurve) + texture(diffuseTex2 , IN.texCoord) * vec4(1,1.5,1,1) * (grassLineCurve) ;
	vec3  bumpNormal    = texture(bumpTex , IN.texCoord * 3 ).rgb * (1-grassLineCurve) + texture(bumpTex2 , IN.texCoord ).rgb * (grassLineCurve);
	bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));


	vec4 temp = vec4(0,0,0,0);

	for(int i = 0 ; i < lightnum ; i++){
		vec3 norDir  = normalize(lightDirection[i]);
		vec3  incident   = lightType[i] == 1 ? -norDir : normalize(lightPos[i] - IN.worldPos) ;
		vec3  halfDir    = normalize(incident + viewDir );
		float  lambert      = max(dot(incident , bumpNormal), 0.0f);
		float  dis    = length(lightPos[i]  - IN.worldPos );

		float  invDistRatio = lightType[i] == 1 ? (lightColour[i].w / 30000.0f) : (lightType[i] == 0 ? lightRadius[i] / (33*dis) : (lightType[i] == 2 ? IN.colour.w  * 33 /(lightRadius[i]* sqrt(dis)) : IN.colour.w / (3*sqrt(dis))));
		float  attenuation  =  lightColour[i].w * invDistRatio * invDistRatio;  // 1.0 - clamp(distance / lightRadius , 0.0,  1.0);

		float  specFactor   = clamp(dot(halfDir , bumpNormal) ,0.0 ,1.0);
		specFactor    = pow(specFactor , 60.0 );

		float intensity = attenuation;
		if(lightType[i] == 2){
			float n = dot(norDir,normalize(IN.worldPos - lightPos[i]));
			intensity = n > cos(lightRadius[i]/360) ? 1 : 0;
		}
		else if(lightType[i] == 3){
			float n = length((IN.worldPos - lightPos[i]) - dot(norDir,IN.worldPos - lightPos[i]) * norDir);
			intensity = n < lightRadius[i] ? 1 : 0;
		}

		vec3  surface = (diffuse.rgb * lightColour[i].rgb);
		temp.rgb += surface * lambert * attenuation * intensity;
		temp.rgb += ((lightColour[i].rgb ) * specFactor )* attenuation *0.01 * intensity;
		temp.rgb +=  surface * 0.0f;    // ambient!
	}
	fragColour = vec4(clamp(temp.r,0.0,1.0),clamp(temp.g,0.0,1.0),clamp(temp.b,0.0,1.0),1);
	fragColour.a = IN.colour.a;
	//fragColour = vec4(lightDirection[0],1);
	//if(IN.worldPos.y > grassLine) fragColour = vec4(1,1,1,1);
}