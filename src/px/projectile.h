// name: projectile.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#pragma once

#include "color.h"
#include "vector.h"

#include <functional>

namespace px
{
	struct projectile
	{
	public:
		typedef unsigned int image_t;
		typedef color color_t;
		typedef double timespan_t;
		typedef std::function<vector(timespan_t time)> position_fn;

	public:
		image_t image;
		color_t color;
		color_t light;

		position_fn position;

	public:
		projectile(image_t img, color_t color_value, position_fn pos_fn) : 
			image(img),
			color(color_value),
			position(pos_fn),
			light(0, 0, 0, 0)
		{}
	};
}