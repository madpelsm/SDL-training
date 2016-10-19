#version 330


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 normal;

uniform mat4 proj;
uniform mat4 camera;

uniform vec3 lightPos;
uniform vec3 lightParams;
uniform float diffuseFactor = 0.08;

out vec3 theColor;
void main()
{
	vec4 projectedPos = proj*camera*vec4(inPosition, 1.0);
	gl_Position =projectedPos;
	vec3 lightDir = (inPosition - lightPos);
	float distance = length(lightDir);
	vec3 lightIntensity = ( -1.0/(1+distance) )*normalize(lightDir);
	theColor=inColor*(diffuseFactor+min(1.00,length(lightParams)*dot(lightIntensity,normalize(normal))));
}