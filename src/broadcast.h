// name: broadcast.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#pragma once

#include "color.h"
#include "point.h"

#include <string>

namespace px
{
		struct broadcast
		{
		public:
			typedef std::string string_t;

		public:
			string_t text;
			color colour;
			double size;
			point position;

		private:
			void init(string_t str, color c, point pos, double size_value) { text = str; colour = c; size = size_value; position = pos; }
			void init(string_t str, color c, point pos) { init(str, c, pos, 1.0); }
			void init(string_t str, point pos) { init(str, color(1, 1, 1), pos); }
			void init(string_t str) { init(str, { 0, 0 }); }

		public:
			broadcast() { init(""); }
			broadcast(string_t str, color c, point pos) { init(str, c, pos); }
			broadcast(string_t str, color c, point pos, double size) { init(str, c, pos, size); }
		};
}