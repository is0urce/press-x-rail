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
		appearance(_I img, color colour) : size(1), image(img), color(colour) {}
		appearance(_I img) : size(1), image(img), color(1, 1, 1) {}
		appearance() : size(1), image{}, color(1, 1, 1) {}

	public:
		_I image;
		color color;
		float size;
	};
}