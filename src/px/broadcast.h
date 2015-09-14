// name: broadcast.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#ifndef PX_BROADCAST_H
#define PX_BROADCAST_H

#include <px/color.h>
#include <px/point.h>

#include <string>

namespace px
{
	struct broadcast
	{
	public:
		typedef std::string string_t;
		typedef color color_t;

	public:
		string_t text;
		color_t color;
		double size;
		point position;

	private:
		void init(string_t str, color_t c, point pos, double size_value) { text = str; color = c; size = size_value; position = pos; }
		void init(string_t str, color_t c, point pos) { init(str, c, pos, 1.0); }
		void init(string_t str, point pos) { init(str, color_t(1, 1, 1), pos); }
		void init(string_t str) { init(str, { 0, 0 }); }

	public:
		broadcast() { init(""); }
		broadcast(string_t str, color_t c, point pos) { init(str, c, pos); }
		broadcast(string_t str, color_t c, point pos, double size) { init(str, c, pos, size); }
	};
}

#endif