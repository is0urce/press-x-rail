// name: notification.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#pragma once

#include "color.h"
#include "vector.h"

#include <string>

namespace px
{
	namespace shell
	{
		struct notification
		{
		public:
			typedef std::string string_t;

		public:
			string_t text;
			color colour;
			double size;
			vector position;

			double phase;

		private:
			void init(string_t str, color c, vector pos, double size_value) { text = str; colour = c; size = size_value; position = pos; }
			void init(string_t str, color c, vector pos) { init(str, c, pos, 1.0); }
			void init(string_t str, vector pos) { init(str, color(1, 1, 1), pos); }
			void init(string_t str) { init(str, { 0, 0 }); }

		public:
			notification() { init(""); }
			notification(string_t str, color c, vector pos) { init(str, c, pos); }
			notification(string_t str, color c, vector pos, double size) { init(str, c, pos, size); }
		};
	}
}