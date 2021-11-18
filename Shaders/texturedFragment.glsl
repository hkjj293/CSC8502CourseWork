#version 330 core

uniform  sampler2D  diffuseTex;
uniform int isFinal;

in Vertex {
	vec2  texCoord;
} IN;

out vec4 fragColour;

void main(void)	{
	
	//float width = 0.01;
	//float rate = 10;
	//rate = 1/rate;
	vec4 value = texture(diffuseTex , IN.texCoord);
	if(isFinal == 0){
		value = value - vec4(0.5,0.7,0.7,0);
	}else{
		if(IN.texCoord.x < 0.01 || IN.texCoord.x > 0.99) value = vec4(0,0,0,1);
		if(IN.texCoord.y < 0.01 || IN.texCoord.y > 0.99) value = vec4(0,0,0,1);
	}
	//if ((IN.texCoord.x - rate * floor(IN.texCoord.x / rate))  > width && (IN.texCoord.y - rate * floor(IN.texCoord.y / rate))  > width){
	//	discard;
	//}
//	if(value.a < 0.7f){
//		discard;
//	}

	fragColour = value;

}