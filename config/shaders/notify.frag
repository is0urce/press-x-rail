#version 330

smooth in vec4 theColor;
smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D img;
uniform float phase;

void main()
{
	vec4 s = vec4(theColor.rgb, theColor.a * texture(img, theTexture.xy).r);
	outputColor = vec4(s.rgb, mix(s.a, 0, min(1, phase)));
}