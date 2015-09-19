#version 330

smooth in vec4 theColor;
smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D glyphTexture;

void main()
{
	vec4 pix = theColor;
	pix.a *= texture(glyphTexture, theTexture.xy).r;
	outputColor = pix;
}