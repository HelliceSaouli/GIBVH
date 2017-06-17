#version 450

// Gaussien blur to do later http://callumhay.blogspot.com/2010/09/gaussian-blur-shader-glsl.html

in vec2 texCoord0;
out vec4 color;

uniform sampler2D renderedTexture;

void main()
{

	 

	color = texture2D(renderedTexture,texCoord0);
}