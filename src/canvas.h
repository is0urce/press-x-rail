// name: panel.h
// type: c++
// desc: class declaration
// auth: isource

#pragma once

#include "point.h"
#include "map.h"
#include "symbol.h"

namespace px
{
	namespace ui
	{
		class canvas : public map<symbol>
		{
		private:
			color m_front;
			color m_back;

		public:
			canvas(point range);
			virtual ~canvas();

		public:
			void text_color(color front_color);
			void draw_color(color back_color);

			void cls();
			void cls(const symbol &fill);
			void rectangle(const point &start, const point &range);
			void rectangle(const point &start, const point &range, const color& back);
			void pset(const point &position);
			void pset(const point &position, const color &point_color);

			void write(const point &position, symbol::code_t code);
			void write(const point &position, symbol::code_t code, const color &front);
			void write(point position, const std::string &text);
			void write(point position, const std::string &text, const color &front);
			void write_integer(point position, int x, const color &front);
		};
	}
}