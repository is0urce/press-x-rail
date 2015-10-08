#version 330

layout(location = 0) in vec2 position_curr;
layout(location = 1) in vec2 position_prev;
layout(location = 2) in vec2 texture;
layout(location = 3) in vec4 color;

smooth out vec4 theColor;
smooth out vec2 theTexture;

uniform float scale;
uniform float aspect;
uniform vec2 center;
uniform float phase;

void main()
{
	vec2 pos = mix(position_prev, position_curr, phase);
	gl_Position = vec4((pos - center) * vec2(1, aspect) * scale, 0, 1);
    theColor = color;
    theTexture = texture;
}