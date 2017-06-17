#version 450

attribute vec3 position;
attribute vec3 color;
attribute vec2 texCoord;

out vec2 texCoord0;


void main()
{
	gl_Position = vec4(position, 1.0);
	texCoord0 = texCoord;

}