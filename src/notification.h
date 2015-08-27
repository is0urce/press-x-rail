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
			void init(double size_value) { phase = 0; size = size_value; }
		public:
			notification() : text(""), colour(1, 1, 1) { init(1); }
			notification(string_t str, color c, vector pos) : text("") { init(1); }
			notification(string_t str, color c, double size, vector pos) : text("") { init(size); }
		};
	}
}