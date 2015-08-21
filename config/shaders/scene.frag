#version 330

smooth in vec2 theTexture;

out vec4 outputColor;
uniform sampler2D img;
uniform vec3 rng;

// noise functions

highp float rand2(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

float rand2lo(vec2 co)
{
    float a = 12.9898;
    float b = 78.233;
    float c = 43758.5453;
    float dt= dot(co.xy ,vec2(a,b));
    float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

vec3 noise3(vec3 seed)
{
	return vec3
	(
		rand2(vec2(gl_FragCoord.x, rand2(vec2(seed.r, gl_FragCoord.y)))),
		rand2(vec2(gl_FragCoord.y, rand2(vec2(seed.g, gl_FragCoord.x)))),
		rand2(vec2(rand2(gl_FragCoord.xy), seed.b))
	);
}

// main


void main()
{
	outputColor = vec4(texture(img, theTexture.xy).rgb + noise3(rng) / 64, 1);
}