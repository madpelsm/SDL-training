#version 330

smooth in vec3 theColor;


void main()
{
	gl_FragColor = vec4(theColor, 1.0);
}