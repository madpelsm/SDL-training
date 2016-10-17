#version 330


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

uniform mat4 trans;
uniform mat4 proj;
uniform mat4 camera;

out vec3 theColor;

void main()
{
	gl_Position = proj*cam*trans*vec4(inPosition, 1.0);
	theColor = inColor;
}