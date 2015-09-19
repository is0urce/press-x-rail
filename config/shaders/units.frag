#version 330

smooth in vec4 theColor;
smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D img;

void main()
{
	outputColor = vec4(theColor.rgb, theColor.a * texture(img, theTexture.xy).r);
}