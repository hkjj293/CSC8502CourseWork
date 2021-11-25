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
	float grassLineCurve = clamp(pow(1.2,IN.worldPos.y - grassLine* abs(normalize(IN.normal).y)),0.0,1.0);
	grassLineCurve = grassLineCurve * abs(normalize(IN.normal).y);
	vec3  viewDir    = normalize(cameraPos  - IN.worldPos );

	for(int i = 0 ; i < lightnum ; i++){
		vec3  incident   = lightType[i] == 1 ? normalize(lightDirection[i] - vec3(0,0,0)) : normalize(lightPos[i] - IN.worldPos) ;
		vec3  halfDir    = normalize(incident + viewDir );
		mat3  TBN        = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

		vec4  diffuse    = texture(diffuseTex , IN.texCoord * 3 ) * (1-grassLineCurve) + texture(diffuseTex2 , IN.texCoord) * vec4(1,1.5,1,1) * (grassLineCurve) ;
		vec3  bumpNormal    = texture(bumpTex , IN.texCoord * 3 ).rgb * (1-grassLineCurve) + texture(bumpTex2 , IN.texCoord ).rgb * (grassLineCurve);

		bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

		float  lambert      = max(dot(incident , bumpNormal), 0.0f);
		float  distance     = length(lightPos[i]  - IN.worldPos );
		float  invDistRatio = lightType[i] == 1 ? 1 : (lightType[i] == 0 ? lightRadius[i] / distance : IN.colour.w * 10 /(lightRadius[i]* sqrt(distance)));
		float  attenuation  =  lightColour[i].w * invDistRatio * invDistRatio;  // 1.0 - clamp(distance / lightRadius , 0.0,  1.0);

		float  specFactor   = clamp(dot(halfDir , bumpNormal) ,0.0 ,1.0);
		specFactor    = pow(specFactor , 60.0 );

		vec2 fl = vec2((sin(bumpNormal.x * 20)+2)/3,(sin(bumpNormal.z * 20)+1)/2);
		float vl = 0;//(sin((viewDir.x * viewDir.z) * 100)+2)/3;

		float intensity = 1;

		if(lightType[i] == 2){
			float n = dot(normalize(lightDirection[i]),normalize(IN.worldPos - lightPos[i] ));
			intensity = n > cos(lightRadius[i]) ? n : 0;
		}
		else if(lightType[i] == 3){
			
		}

		vec3  surface = (diffuse.rgb * lightColour[i].rgb);
		fragColour.rgb += surface * lambert * attenuation * intensity;
		fragColour.rgb += ((lightColour[i].rgb ) * specFactor )* attenuation *0.01 * intensity;
		fragColour.rgb +=  surface * 0.0f;    // ambient!
	}
	fragColour.a = IN.colour.a;
	//fragColour = vec4(lightDirection[0],1);
	//if(IN.worldPos.y > grassLine) fragColour = vec4(1,1,1,1);
}