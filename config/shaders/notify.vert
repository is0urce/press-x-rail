#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture;
layout(location = 2) in vec4 color;

smooth out vec4 theColor;
smooth out vec2 theTexture;

uniform float scale;
uniform float aspect;
uniform vec2 center;
uniform float phase;

void main()
{
	vec2 pos = (position - center) + vec2(0, phase);
	gl_Position = vec4(pos * vec2(1, aspect) * scale, 0, 1);
    theColor = color;
    theTexture = texture;
}