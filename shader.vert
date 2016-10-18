#version 330


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 normal;

uniform mat4 proj;
uniform mat4 camera;

uniform vec4 lightPos;
uniform vec4 lightParams;

out vec3 endColor;
void main()
{

	gl_Position =proj*camera*vec4(inPosition, 1.0);

	vec3 lightDir = inPosition - lightPos;
	float distance = dot(lightDir,lightDir);
	vec3 lightIntensity = ( 1/ ( 1+sqrt(distance) ) )*normalize(lightDir);
	endColor = lightParams*dot( normalize(normal) * lightIntensity );
}