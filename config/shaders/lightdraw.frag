#version 330

smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D lmtexture;

void main()
{
	outputColor = vec4(texture(lmtexture, theTexture.xy).rgb, 1.0);
}