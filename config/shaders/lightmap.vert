#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

uniform float tsize; // 1 / texture_size * 2

smooth out vec4 theColor;

void main()
{
	theColor = color;
	gl_Position = vec4(position * tsize + vec2(-1, -1), 0, 1); 
}