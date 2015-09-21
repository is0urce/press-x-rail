#version 330

smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D lmtexture;
uniform sampler2D lmprevtexture;
uniform float phase;

void main()
{
	vec3 lm = texture(lmtexture, theTexture.xy).rgb;
	vec3 lmprev = texture(lmprevtexture, theTexture.xy).rgb;
	outputColor = vec4(mix(lmprev, lm, clamp(phase, 0, 1)), 1.0);
}