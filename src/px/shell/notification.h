// name: notification.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#ifndef PX_SHELL_NOTIFICATION_H
#define PX_SHELL_NOTIFICATION_H

#include <px/color.h>
#include <px/vector.h>

#include <string>

namespace px
{
	namespace shell
	{
		struct notification
		{
		public:
			typedef std::string string_t;
			typedef color color_t;

		public:
			string_t text;
			color_t color;
			double size;
			vector position;

			double phase;

		private:
			void init(string_t str, color_t c, vector pos, double size_value) { text = str; color = c; size = size_value; position = pos; }
			void init(string_t str, color_t c, vector pos) { init(str, c, pos, 1.0); }
			void init(string_t str, vector pos) { init(str, color_t(1, 1, 1), pos); }
			void init(string_t str) { init(str, { 0, 0 }); }

		public:
			notification() { init(""); }
			notification(string_t str, color_t c, vector pos) { init(str, c, pos); }
			notification(string_t str, color_t c, vector pos, double size) { init(str, c, pos, size); }
		};
	}
}

#endif