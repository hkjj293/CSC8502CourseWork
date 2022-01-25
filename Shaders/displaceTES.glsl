#version 400

layout(quads, equal_spacing , cw) in;

//uniform sampler2D heightMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float rot;
uniform int level;

in Vertex { //Sent from the TCS
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} IN[]; // Equal to TCS layout size

out Vertex { //Each TES works on a single vertex!
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} OUT;

vec3 QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d) {
	vec3 p0 = mix(a,c, gl_TessCoord.x);
	vec3 p1 = mix(b,d, gl_TessCoord.x);

	return mix(p0 ,p1 ,gl_TessCoord.y);
}

vec2 QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
	vec2 p0 = mix(a,c, gl_TessCoord.x);
	vec2 p1 = mix(b,d, gl_TessCoord.x);

	return mix(p0 ,p1 ,gl_TessCoord.y);
}

void main() {
	vec3 combinedPos = QuadMixVec3( gl_in [0]. gl_Position.xyz ,
		gl_in [1]. gl_Position.xyz ,
		gl_in [2]. gl_Position.xyz ,
		gl_in [3]. gl_Position.xyz );

	OUT.texCoord = QuadMixVec2(IN[0]. texCoord ,
	IN[1]. texCoord ,
	IN[2]. texCoord ,
	IN[3]. texCoord );
	float xPeriod = 100 ;
	float xSpeed = 5;
	float zPeriod = 100;
	float zSpeed = 5;
	vec4 worldPos = modelMatrix * vec4(combinedPos , 1);
	float a = radians(xSpeed * rot)+ xPeriod * worldPos.x;
	float b = radians(3 * rot)+(worldPos.x +worldPos.z)*10;
	float c = radians(zSpeed * rot)+ zPeriod * worldPos.z;
	float height = (sin(c) + sin(c))/2.0f; //texture(heightMap , OUT.texCoord ).x;
	float heightScale = 1;
	worldPos.y += heightScale * height;
	OUT.worldPos = worldPos.xyz;
	OUT.colour = vec4(QuadMixVec3(IN[0].colour.xyz ,
		IN[1].colour.xyz ,
		IN[2].colour.xyz ,
		IN[3].colour.xyz), 1);
	OUT.normal = normalize(vec3(0*heightScale *cos(a) * (xSpeed/(6.28*xPeriod)), 1, cos(c) ));

	gl_Position = projMatrix * viewMatrix * worldPos;
}