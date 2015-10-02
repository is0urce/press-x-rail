// name: canvas.h
// type: c++
// desc: class declaration
// auth: isource

#ifndef PX_UI_CANVAS_H
#define PX_UI_CANVAS_H

#include <px/point.h>
#include <px/map.h>
#include <px/rectangle.h>
#include <px/ui/symbol.h>

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
			void rectangle(const point &start, const point &range, const color &back);
			void rectangle(px::rectangle bounds, const color &back);
			void pset(const point &position);
			void pset(const point &position, const color &point_color);

			void write(const point &position, symbol::code_t code);
			void write(const point &position, symbol::code_t code, const color &front);
			void write(point position, const std::string &text);
			void write(point position, const std::string &text, const color &front);
			void write_integer(point position, int x);
			void write_integer(point position, int x, const color &front);
		};
	}
}

#endif