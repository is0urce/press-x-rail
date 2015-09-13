// name: appearance.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "color.h"

namespace px
{
	template <typename _I>
	class appearance
	{
	public:
		appearance(_I img, color c, double size_value) : size(size_value), image(img), color(c) {}
		appearance(_I img, color c) : size(1), image(img), color(c) {}
		appearance(_I img) : size(1), image(img), color(1, 1, 1) {}
		appearance() : size(1), image{}, color(1, 1, 1) {}

	public:
		_I image;
		color color;
		double size;
	};
}