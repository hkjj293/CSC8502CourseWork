#version 330 core

uniform sampler2D sceneTex;
uniform int intensity;
uniform int isVertical;

in Vertex {
	vec2  texCoord;
} IN;

out vec4 fragColor;

//const float scaleFactors [7] = float [](0.006 ,  0.061,  0.242,  0.383,  0.242,  0.061,  0.006);

void main(void)	{
	if(IN.texCoord.x < 0.01 || IN.texCoord.x > 0.99) discard;
	if(IN.texCoord.y < 0.01 || IN.texCoord.y > 0.99) discard;
//	fragColor = texture(sceneTex, IN.texCoord);//vec4 (0,0,0,1);
//	//vec2  delta = vec2 (0,0);
	vec2 dy = dFdy(IN.texCoord);
	vec2 dx = dFdx(IN.texCoord);

	fragColor = (texture(sceneTex, IN.texCoord) + texture(sceneTex, IN.texCoord.xy + (dx*intensity) +(dy*intensity))) / 2;
//	for(int i = 0; i < 3; i++){
//		for(int j = 0; j < 3; j++){
//			vec2 offsetX = dx * (j-1);
//			vec2 offsetY = dy * (i-1);
//			vec4 tmp = texture(sceneTex, IN.texCoord.xy + offsetX + offsetY);
//			fragColor += (tmp * (j - 1));
//		}
//	}
//	for(int i = 0; i < 3; i++){
//		for(int j = 0; j < 3; j++){
//			vec2 offsetX = dx * (j-1);
//			vec2 offsetY = dy * (i-1);
//			vec4 tmp = texture(sceneTex, IN.texCoord.xy + offsetX + offsetY);
//			fragColor += (tmp * (i - 1));
//		}
//	}
//	float avg = (fragColor.r + fragColor.g + fragColor.b)/3;
//	//fragColor.a = 
//	avg = avg + 0.5;
//	avg = avg * avg;
//	avg = avg - 0.3;
//	fragColor = vec4(avg,avg,avg,1); // + texture(sceneTex, IN.texCoord));


	//if(isVertical  == 1) {
	//	delta = dFdy(IN.texCoord);
	//}
	// else{
	//	delta = dFdx(IN.texCoord);
	// }
	 //for(int i = 0; i < 7; i++){
		//vec2 offset = delta * (i - 3);
		//vec4 tmp = texture(sceneTex, IN.texCoord.xy + offset);
		//fragColor += tmp * scaleFactors[i];
	 //}
}