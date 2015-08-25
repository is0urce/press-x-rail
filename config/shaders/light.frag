#version 330

smooth in vec4 thePosition;

uniform float inner;
uniform float outerinv;
uniform vec4 pos;
uniform vec4 col;

out vec4 outputColor;

float sat(float v)
{
	return clamp(v, 0.0, 1.0);
}

// d - distance
// i - size of inner zone
// r - reach inverted
float fade(float distance, float i, float oinv)
{
	//return 1.0 / (pow(distance, 2) + 1.0);
	float d = max(distance, i);
    return sat(1.0 - pow(d * oinv, 4.0)) / (d * d + 1.0);
}

void main()
{
	float d = length(thePosition - pos);
	float f = fade(d, inner, outerinv);

	outputColor = vec4(col.rgb * f, 0.0);
}