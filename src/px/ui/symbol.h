// name: symbol.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#pragma once

#include <px/color.h>

namespace px
{
	namespace ui
	{
		struct symbol
		{
		public:
			typedef unsigned int code_t;

		public:
			code_t code;
			color front;
			color back;

		public:
			symbol(code_t u_plus, color symbol_color, color background_color) : code(u_plus), front(symbol_color), back(background_color) {}
			symbol(code_t u_plus, color symbol_color) : code(u_plus), front(symbol_color), back(0, 0, 0, 0) {}
			symbol(code_t u_plus) : code(u_plus), front(1, 1, 1, 1), back(0, 0, 0, 0) {}
			symbol() : code(' '), front(1, 1, 1, 1), back(0, 0, 0, 0) {}
		};
	}
}