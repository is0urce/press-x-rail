#version 330

layout(location = 0) in vec2 position;

uniform float scale;
uniform float aspect;
uniform vec2 center;

smooth out vec4 thePosition;

void main()
{
	vec2 pos = vec2((position - center) * vec2(1, aspect) * scale);
	thePosition = vec4(position, 0, 1); // world
	gl_Position = vec4(pos, 0, 1); // screen
}