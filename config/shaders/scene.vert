#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture;

smooth out vec2 theTexture;

void main()
{
    theTexture = texture;
	gl_Position = vec4(position, 0, 1);
}